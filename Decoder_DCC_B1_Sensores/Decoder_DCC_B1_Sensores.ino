/* Modulo Bucle Retorno 1 || 10-04-2026

  # Desvio 1   Entrada cocheras    Dir=  5;  Subdir= 0;  F=1;
  --------------------------------------------------------
  # Animacion soldador             Dir=  2;  Subdir= 1;  F=2;
  # Luces Exteriores               Dir=  1;  Subdir= 1;  F=3;
  # Luces Casas                    Dir=  1;  Subdir= 2;  F=4;
  # Semáforo Desvío principal      Dir=  5;  Subdir= 1;  F=18;
  --------------------------------------------------------
  Descipción: 
  Decoder de accesorios basado en la librería DCC Decoder de MynaBay
  https://github.com/MynaBay/DCC_Decoder, módulo transmisor RF 433 que 
  se conecta con la pantalla tft 3.5" conectada a la central DCC donde
  se muestran el diagrama de la pista. 
  --------------------------------------------------------
  Distribucion de pines:

  D2 = Entrada DCC
  D3 = Sensor IR Loop A
  D4 = Sensor IR Loop B
  D5 = Luces farolas (PWM)
  D6 = Luces interior casas (PWM)
  D7 = Pin Tx RF433  vw_tx_pin = 7 (módulo FS1000A)
  D8 = Luz semáforo - rojo
  D9 = Luz semáforo - verde
  D10 = Rele corazón desvío cocheras
  D11 = Relé Bucle de retorno
  D12 = Soldador
  D13 = 
  A0 = Sensor Bloque 1 E/S Sur SENS03
  A1 = Sensor Bloque 2 Interior bucle SENS04
  A2 = Sensor Bloque 3 E/S Norte SENS05
  A3 = Sensor Bloque 4 NO USADO
*/
#include <VirtualWire.h>      // Librería para RF433
#include "listaAccesorios.h"  // Librería con la descripción de accesorios
#include "listaDetectores.h"  // Librería con la descripción de sensores
#include <DCC_Decoder.h>      // Librería DCC_Decoder


#define kDCC_INTERRUPT 0  // Número de interrupción

// definimos los pines
//////////////////////
// #define SENSOR_IR_A 3  // pin Modulo Ir A Sur
// #define SENSOR_IR_B 4  // pin Modulo Ir B Norte
#define P_FAROLAS 5    // Luces Farolas (PWM)
#define P_CASAS 6      // Luces interior casas (PWM)
// #define RF_TX_PIN      7   // Pin Tx RF433  vw_tx_pin = 7
#define SEM_PRIN_VERDE 8  // Led semáforo verde
#define SEM_PRIN_ROJO 9   // Led semáforo verde
#define RELECORAZON 10    // Rele corazón desvío cocheras
#define RELELOOP 11       // pin Rele
#define SOLDADOR 12       // Efecto soldador

// Número indice accItem (acc)
#define ACC_LUCES_EXTERIOR 0
#define ACC_LUCES_INTERIOR 1
#define ACC_SOLDADOR 2
#define DESVIO_PRINCIPAL 3  // Semaforo
#define DESVIO_COCHERAS 4   // Rele corazón desvío

// Número de accesorios
#define MAX_ACC 5

////////////////////////////////
// Animacion Luces Progresivas
bool funcion_luces_inicio = true;
bool funcion_luces_progresivo = false;
bool funcion_luces_encendidas = false;

bool desvio_cocheras_flag = false;
const uint8_t inicia_luces_pwm = 10;  // Intensidad inicial de las luces
const uint8_t final_luces_pwm = 110;  // Intensidad final de las luces
const uint8_t activa_luces_pwm = 200;
uint8_t encendidoLuces = inicia_luces_pwm;
unsigned long previousMillisLuces;
unsigned long currentMillisLuces;
const unsigned long periodoLuces = 40;

////////////////////////////////
// Animacion soldador
int intesisdadSoldador = 175;  // Intensidad para pin PWM soldador
const long duracionFlashMin = 5;
const long duracionFlashMax = 50;
const long tiempoReposoMin = 2000;
const long tiempoReposoMax = 10000;
long S1Interval = 500;
long S2Interval = 500;
unsigned long S1previousMillis = 0;
unsigned long S2previousMillis = 0;
boolean estadoSoldador0 = false;
boolean estadoSoldador1 = true;

///////////////////////////////
// Timer Sensores
unsigned long previousMillisSensores = 0;
const long intervalSensores = 50;  // Tiempo para checheo de sensores (millisegundos)
#define MAX_SENSORES 6

struct Sensores {
  uint8_t id;
  uint8_t pin;
  bool estado;
  bool old_estado;
};
Sensores sensor[MAX_SENSORES];

void ConfigureSensors()
{
  sensor[0].id = 1;
  sensor[0].pin = A0;
  sensor[0].estado = false;
  sensor[0].old_estado = false;

  sensor[1].id = 2;
  sensor[1].pin = A1;
  sensor[1].estado = false;
  sensor[1].old_estado = false;

  sensor[2].id = 3;
  sensor[2].pin = A2;
  sensor[2].estado = false;
  sensor[2].old_estado = false;

  sensor[3].id = 4;
  sensor[3].pin = A3;
  sensor[3].estado = false;
  sensor[3].old_estado = false;
  
  sensor[4].id = 5;
  sensor[4].pin = 3;
  sensor[4].estado = false;
  sensor[4].old_estado = false;

  sensor[5].id = 6;
  sensor[5].pin = 4;
  sensor[5].estado = false;
  sensor[5].old_estado = false;

}
bool statusrelay = true;

struct accItem {
  int direccion;    // Dirección de respuesta DCC
  boolean output;   // Estado del accesorio: 1=on, 0=off (ECoS: on=recto, off=desviado)
  byte outputPin0;  // Pin de salida Arduino para una función adicional (No usar con servo)
  byte outputPin1;  // Pin de salida Arduino para una función adicional (No usar con servo)
};
accItem acc[MAX_ACC];

//////////////////////////////////////////////////////////////
// Configuración de funciones del decodificador
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ConfigureFunctionsAndServos() 
{
  //////////////////////
  acc[ACC_LUCES_EXTERIOR].direccion = LUCES_EXTERIORES; // Direccion 2 Luces Farolas. Dir 1 Sdir 1
  acc[ACC_LUCES_EXTERIOR].outputPin0 = P_FAROLAS;       // Pin 5 Luces farolas
  acc[ACC_LUCES_EXTERIOR].output = false;               // Estado inicial
  //////////////////////
  acc[ACC_LUCES_INTERIOR].direccion = LUCES_CASAS;      // Direccion 3 Luces Interiores. Dir 1 Sdir 2
  acc[ACC_LUCES_INTERIOR].outputPin0 = P_CASAS;         // Pin 6 Luces Interiores
  acc[ACC_LUCES_INTERIOR].output = false;               // Estado inicial
  /////////////////////
  acc[ACC_SOLDADOR].direccion = EF_SOLDADOR_B1;         // Direccion 6; Dir 2 Sdir 1
  acc[ACC_SOLDADOR].outputPin0 = SOLDADOR;              // Pin 12 funcion soldador
  acc[ACC_SOLDADOR].output = false;                     // Estado inicial
  /////////////////////
  acc[DESVIO_PRINCIPAL].direccion = B1_PRINCIPAL;       // Direccion 17; Desvio Norte-este taller 2 ; Dir 5 Sdir 0
  acc[DESVIO_PRINCIPAL].outputPin0 = SEM_PRIN_VERDE;    // Pin 8 semáforo verde
  acc[DESVIO_PRINCIPAL].outputPin1 = SEM_PRIN_ROJO;     // Pin 9 semáforo rojo
  acc[DESVIO_PRINCIPAL].output = false;                 // Estado inicial
  /////////////////////
  acc[DESVIO_COCHERAS].direccion = B1_COCHERAS;         // Direccion 18; Desvio Norte-este taller 2 ; Dir 5 Sdir 1
  acc[DESVIO_COCHERAS].outputPin0 = RELECORAZON;        // Pin 10 Relé corazon desvío
  acc[DESVIO_COCHERAS].output = false;                   // Estado inicial

}  // END ConfigureFunctionsAndServos()

/////////////////////////////////////////
// DCC packet handler Gestión de paquetes
/////////////////////////////////////////
void BasicAccDecoderPacket_Handler(int address, boolean activate, byte data)
{
  // conversión de direcciones NMRA a un formato humano
  address -= 1;
  address *= 4;
  address += 1;
  address += (data & 0x06) >> 1;

  boolean enable = (data & 0x01) ? 1 : 0;
  Serial.print(F("Direccion de accesorio: "));
  Serial.println(address);
  for (int i = 0; i < MAX_ACC; i++) {
    if (address == acc[i].direccion) {
      if (enable) acc[i].output = true;
      else acc[i].output = false;
      // printMessage(i);  // DEBUG ONLY
      break;
    }
  }
}

///////////////////////////////
// Setup (se ejecuta al inicio)
void setup() 
{
  delay(1000);
  Serial.begin(115200);
  Serial.println(F("DECODER 1_0 Sensores"));

  DCC.SetBasicAccessoryDecoderPacketHandler(BasicAccDecoderPacket_Handler, true);

  ConfigureFunctionsAndServos();
  ConfigureSensors();


  for (int i = 0; i < MAX_ACC; i++) 
  {
    pinMode(acc[i].outputPin0, OUTPUT);
    pinMode(acc[i].outputPin1, OUTPUT);

    digitalWrite(acc[i].outputPin0, HIGH);
    digitalWrite(acc[i].outputPin1, HIGH);

    delay(300);
    digitalWrite(acc[i].outputPin0, LOW);
    digitalWrite(acc[i].outputPin1, LOW);
  }
  DCC.SetupDecoder(0x00, 0x00, kDCC_INTERRUPT);
  pinMode(2, INPUT_PULLUP);  //Interrupt 0 with internal pull up resistor (can get rid of external 10k)

  // Pines de entrada
  for (int Nsensor = 0; Nsensor < MAX_SENSORES; Nsensor++) 
  {
    Serial.println(Nsensor);
    pinMode(sensor[Nsensor].pin, INPUT_PULLUP);
  }
  
  // Pines de salida
  pinMode(RELELOOP, OUTPUT);
  pinMode(RELECORAZON, OUTPUT);
  digitalWrite(RELELOOP, LOW);
  digitalWrite(RELECORAZON, LOW);

  // Setup RF433 FS1000A
  vw_set_ptt_inverted(true);  // Requerido para FS1000A y DR3100 RF433
  vw_setup(2000);             // Bits por seg
}

////////////////////////////////////////////
// loop principal (se ejecuta continuamente)
////////////////////////////////////////////
void loop()
{
  DCC.loop();
  // Temporizador para la detección de sensores
  unsigned long currentMillisSensores = millis();
  if (currentMillisSensores - previousMillisSensores >= intervalSensores) 
  {
    previousMillisSensores = currentMillisSensores;
    Ocupacion();  // Deteccion.ino
  }
  Funciones();  // Funciones.ino
}  // Fin del loop



/* Modulo Bucle Retorno 1 || 30-04-2024

  # Desvio 1   Entrada cocheras    Dir=  5;  Subdir= 0;  F=1;
  --------------------------------------------------------
  # Animacion soldador             Dir=  1;  Subdir= 1;  F=2;
  # Luces Exteriores               Dir=  1;  Subdir= 2;  F=3;
  # Luces Casas                    Dir=  1;  Subdir= 3;  F=4;
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
  D10 =
  D11 = Relé Bucle de retorno
  D12 = Soldador
  D13 = Rele corazón desvío cocheras
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
#define SENSOR_IR_A 3  // pin Modulo Ir A Sur
#define SENSOR_IR_B 4  // pin Modulo Ir B Norte
#define P_FAROLAS 5    // Luces Farolas (PWM)
#define P_CASAS 6      // Luces interior casas (PWM)
// #define RF_TX_PIN      7   // Pin Tx RF433  vw_tx_pin = 7
#define SEM_PRIN_VERDE 8  // Led semáforo verde
#define SEM_PRIN_ROJO 9   // Led semáforo verde
#define RELELOOP 11       // pin Rele
#define SOLDADOR 12       // Efecto soldador
#define RELECORAZON 13    // Rele corazón desvío cocheras
#define BLOQUE1PIN A0     // SENS03
#define BLOQUE2PIN A1     // SENS04
#define BLOQUE3PIN A2     // SENS05
#define BLOQUE4PIN A3     // No usado

// Número indice accItem (acc)
#define ACC_LUCES_EXTERIOR 0
#define ACC_LUCES_INTERIOR 1
#define ACC_SOLDADOR 2
#define DESVIO_PRINCIPAL 3  // Semaforo
#define DESVIO_COCHERAS 4   // Rele corazón desvío
// Número de accesorios
///////////////////////
#define MAX_ACC 5

////////////////////////////////
// Animacion Luces Progresivas
bool funcion_luces_inicio = true;
bool funcion_luces_progresivo = false;
bool funcion_luces_encendidas = false;

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
const long intervalSensores = 150;  // Tiempo para checheo de sensores (millisegundos)
bool bloque[8] = { false, false, false, false, false, false, false, false };

bool BloqueStatus[8] = { false, false, false, false, false, false, false, false };
bool statusrelay = true;

struct accItem {
  int direccion;    // DCC address to respond to
  boolean output;   // State of DCC accessory: 1=on, 0=off (ECoS: on=straight, off=turnout)
  byte outputPin0;  // Arduino output pin for additional function (not where servo is attached to)
  byte outputPin1;  // Arduino output pin for additional function (not where servo is attached to)
};
accItem acc[MAX_ACC];

//////////////////////////////////////////////////////////////
// Configuración de funciones del decodificador
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ConfigureFunctionsAndServos() {
  //////////////////////
  acc[ACC_LUCES_EXTERIOR].direccion = LUCES_EXTERIORES;  // Direccion 2 Luces Farolas. Dir 1 Sdir 1
  acc[ACC_LUCES_EXTERIOR].outputPin0 = P_FAROLAS;        // Pin 5 Luces farolas
  acc[ACC_LUCES_EXTERIOR].output = false;                // Estado inicial
  //////////////////////
  acc[ACC_LUCES_INTERIOR].direccion = LUCES_CASAS;  // Direccion 3 Luces Interiores. Dir 1 Sdir 2
  acc[ACC_LUCES_INTERIOR].outputPin0 = P_CASAS;     // Pin 6 Luces Interiores
  acc[ACC_LUCES_INTERIOR].output = false;           // Estado inicial
  /////////////////////
  acc[ACC_SOLDADOR].direccion = EF_SOLDADOR_B1;  // Direccion 6; Dir 1 Sdir 1
  acc[ACC_SOLDADOR].outputPin0 = SOLDADOR;       // Pin 12 funcion soldador
  acc[ACC_SOLDADOR].output = false;              // Estado inicial
  /////////////////////
  acc[DESVIO_PRINCIPAL].direccion = B1_PRINCIPAL;     // Direccion 17; Desvio Norte-este taller 2 ; Dir 6 Sdir 0
  acc[DESVIO_PRINCIPAL].outputPin0 = SEM_PRIN_VERDE;  // Pin 8 semáforo verde
  acc[DESVIO_PRINCIPAL].outputPin1 = SEM_PRIN_ROJO;   // Pin 9 semáforo rojo
  acc[DESVIO_PRINCIPAL].output = false;               // Estado inicial
  /////////////////////
  acc[DESVIO_COCHERAS].direccion = B1_COCHERAS;   // Direccion 17; Desvio Norte-este taller 2 ; Dir 6 Sdir 0
  acc[DESVIO_COCHERAS].outputPin0 = RELECORAZON;  // Pin 13 Relé corazon desvío
  acc[DESVIO_COCHERAS].output = false;            // Estado inicial

}  // END ConfigureFunctionsAndServos()

/////////////////////////////////////////
// DCC packet handler Gestión de paquetes
/////////////////////////////////////////
void BasicAccDecoderPacket_Handler(int address, boolean activate, byte data) {
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
      mensaje(i);  // DEBUG ONLY
      break;
    }
  }
}

///////////////////////////////
// Setup (se ejecuta al inicio)
///////////////////////////////
void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println(F("DECODER 1_0"));

  DCC.SetBasicAccessoryDecoderPacketHandler(BasicAccDecoderPacket_Handler, true);

  ConfigureFunctionsAndServos();

  for (int i = 0; i < MAX_ACC; i++) {
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
  pinMode(SENSOR_IR_A, INPUT_PULLUP);
  pinMode(SENSOR_IR_B, INPUT_PULLUP);
  pinMode(BLOQUE1PIN, INPUT_PULLUP);
  pinMode(BLOQUE2PIN, INPUT_PULLUP);
  pinMode(BLOQUE3PIN, INPUT_PULLUP);
  pinMode(BLOQUE4PIN, INPUT_PULLUP);

  // Pines de salida
  pinMode(RELELOOP, OUTPUT);
  pinMode(RELECORAZON, OUTPUT);
  digitalWrite(RELELOOP, LOW);
  digitalWrite(RELECORAZON, LOW);

  // Setup RF433 FS1000A
  vw_set_ptt_inverted(true);  // Required for DR3100 RF433
  vw_setup(2000);             // Bits per sec
}

////////////////////////////////////////////
// loop principal (se ejecuta continuamente)
////////////////////////////////////////////
void loop() {

  DCC.loop();   // DCC library
  Funciones();  // Funciones.ino

  // Temporizador para la detección de sensores
  unsigned long currentMillisSensores = millis();
  if (currentMillisSensores - previousMillisSensores >= intervalSensores) {
    previousMillisSensores = currentMillisSensores;
    Ocupacion();  // Deteccion.ino
  }
}  // Fin del loop


void mensaje(int option) {
  String msg = "";
  switch (option) {
    case ACC_LUCES_EXTERIOR:
      if (acc[ACC_LUCES_EXTERIOR].output) msg = "*-Luces Exteriores ON";
      else msg = "*-Luces Exteriores OFF";
      break;
    case ACC_LUCES_INTERIOR:
      if (acc[ACC_LUCES_INTERIOR].output) msg = "*-Luces interiores ON";
      else msg = "*-Luces interiores OFF";
      break;
    case ACC_SOLDADOR:
      if (acc[ACC_SOLDADOR].output) msg = "*-Accesorio Soldador ON";
      else msg = "*-Accesorio Soldador OFF";
      break;
  }
  Serial.println(msg);
}

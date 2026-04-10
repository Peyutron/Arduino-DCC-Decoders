/* Modulo Bucle Retorno 1 || 30-04-2024
//
//  #Desvio 1   Entrada cocheras    Dir=  5;  Subdir= 0;  F=17;
//  #Desvio 2   Cocheras            Dir=  5;  Subdir= 1;  F=18;
//  --------------------------------------------------------
//  #Animacion soldador             Dir=  1;  Subdir= 1;  F=2;
//  #Luces Exteriores               Dir=  1;  Subdir= 2;  F=3;
//  #Luces Casas                    Dir=  1;  Subdir= 3;  F=4;
//  --------------------------------------------------------

//////////////////////////////////////////////////////////////////
//Distribucion de pines:
// D2 = Entrada DCC
// D3 = LUCES TALLER
// D4 = NEOPIXEL
// D5 = LUCES FAROLAS
// D6 = LUCES INTERIORES
// D7 = SERVO TALLER 
// D8 = SERVO PRINCIPAL
// D9 = LUCES CARRETERA
// D10 = SEMAFORO 1_1
// D11 = SEMAFORO 1_2
// D12 =
// D13 =
// A0 = 
// A1 =
// A2 =
// A3 =
*/
#include "DCC_Decoder.h"
#include <Adafruit_NeoPixel.h>
#include "listaAccesorios.h"
#include <Servo.h>

// Nombres para los pines
#define kDCC_INTERRUPT  0   // Interrupción DCC
#define DCC_PIN         2   // Entrada señal DCC chip 6N137
#define P_LUCES_TALLER  3   // Pin luces Taller (PWM)
#define PIN_NEO         4   // pin efecto TV DIGITAL
#define P_FAROLAS       5   // Luces Farolas (PWM)
#define P_CASAS         6   // Luces interior casas (PWM)
#define SERVO_TALLER    7   // Desvio Taller (Servo)
#define SERVO_PRINCIPAL 8   // Desvio Principal (Servo)
#define P_LUZ_CARRETERA 9   // Luces carretera (PWM)
#define SEMAFORO1_1     10  // Semáforo talleres 1_1 PWM 
#define SEMAFORO1_2     11  // Semáforo talleres 1_2 PWM


// Número de pixeles que se activan
#define NUMPIXELS       1   // Número de modulos 8211

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN_NEO, NEO_GRB + NEO_KHZ800);

////////////////
// Nombres del array accItem
#define ACC_LUCES_EXTERIOR  0
#define ACC_LUCES_CASAS     1
#define ACC_LUCES_TALLER    2
#define ACC_LUCES_CARRETERA 3
#define ACC_EFECTO_TV       4
#define DESVIO_PRINCIPAL    5
#define DESVIO_TALLER       6

/////////////////////////////
// Fill in these 2 values ...
///////////////////////////////////////////////////////////
#define MAX_ACC 7            // Número máximo de accesorios
const byte servotimer = 60;  // Timer para el cambio de angulo. Valor bajo -> mayor velocidad
/////////////////////////////////////////////////////////////////////////////////////////////

unsigned long timetoupdatesetpoint = millis() + servotimer;
////////////////
// Animacion Luces Progresivas
bool estadoLuces = false;
bool funcionLuces;
const int inicioLuces = 10;  // Intensidad inicial de las luces
const int finalLuces = 150;  // Intensidad final de las luces
int encendidoLuces = inicioLuces;
unsigned long previousMillisLuces = 0;  // Almacena la última actualización
const unsigned long periodoLuces = 40;

////////////////
// Animación TV NEOPIXEL
boolean estadoTV = true;
unsigned long startMillisTV;
unsigned long previousMillisTV = 0;  // Almacena la última actualización
const unsigned long periodoTV = 4000;


struct accItem 
{
  int16_t direccion;    // DCC address to respond to
  boolean output;   // State of DCC accessory: 1=on, 0=off (ECoS: on=straight, off=turnout)
  byte outputPin0;  // Arduino output pin for additional function (not where servo is attached to)
  byte outputPin1;  // Arduino output pin for additional function (not where servo is attached to)
  byte angulo;
  byte setpoint;
  byte anguloOn;
  byte anguloOff;
  Servo servo;
};
accItem acc[MAX_ACC];

///////////////////////////////////////////////////////////////////////////////////////////////////
// Fill in the data for every function and servo.
// COPY - PASTE as many times as you have functions. The amount must be same as in line 22 above!
// A servo is coupled to an accessory[n]. It rotates based on accessory[n].output = 1 (CCW) or 0 (CW)
// If you have multiple servos you need to couple them to different accessories. However ...
// accessories may switch the same output pin (e.g. pin 13, which has the on board led attached)
///////////////////////////////////////////////////////////////////////////////////////////////////
void ConfigureFunctionsAndServos() {
  // Dirección 2; Luces calles; Direccion 1 Subdireccion 1
  acc[ACC_LUCES_EXTERIOR].direccion = LUCES_EXTERIORES;
  acc[ACC_LUCES_EXTERIOR].outputPin0 = P_FAROLAS;
  acc[ACC_LUCES_EXTERIOR].output = false;
  ////////////////
  // Dirección 3; Luces interior edificios; Direccion 1 Subdireccion 2
  acc[ACC_LUCES_CASAS].direccion = LUCES_CASAS;
  acc[ACC_LUCES_CASAS].outputPin0 = P_CASAS;
  acc[ACC_LUCES_CASAS].outputPin1 = P_LUZ_CARRETERA;
  acc[ACC_LUCES_CASAS].output = false;
  ////////////////
  // Direccion 4; Luces talleres; Direccion 1 Subdireccion 3
  acc[ACC_LUCES_TALLER].direccion = LUCES_TALLER;
  acc[ACC_LUCES_TALLER].outputPin0 = P_LUCES_TALLER;
  acc[ACC_LUCES_TALLER].output = false;
  ////////////////
  // Direccion 5; Luces carreteras 2; Direccion 6 Subdireccion 0
  // acc[ACC_LUCES_CARRETERA].direccion = LUCES_CARRETERA;
  // acc[ACC_LUCES_CARRETERA].outputPin0 = P_LUZ_CARRETERA;
  //acc[ACC_LUCES_CARRETERA].output = false;
  ////////////////
  // Direccion 7; Animación televisor; Direccion 2 Subdireccion 2
  acc[ACC_EFECTO_TV].direccion = EF_TELEVISOR_B1;
  acc[ACC_EFECTO_TV].output = false;
  ////////////////
  // Dirección 17; Desvio Norte-Oeste acceso cocheras; Direccion 5 Subdireccion 0
  acc[DESVIO_PRINCIPAL].direccion = B1_PRINCIPAL;
  acc[DESVIO_PRINCIPAL].anguloOff = 100;  // Angulo para DCC=off 120
  acc[DESVIO_PRINCIPAL].anguloOn = 75;    // Angulo para DCC=on 95
  acc[DESVIO_PRINCIPAL].setpoint = 100;
  acc[DESVIO_PRINCIPAL].output = false;
  acc[DESVIO_PRINCIPAL].servo.attach(SERVO_PRINCIPAL);
  ////////////////
  // Direccion 18; Desvio bucle 1 cocheras; Direccion 5 Subdireccion 1
  acc[DESVIO_TALLER].direccion = B1_COCHERAS;
  acc[DESVIO_TALLER].outputPin0 = SEMAFORO1_1;
  acc[DESVIO_TALLER].outputPin1 = SEMAFORO1_2;
  acc[DESVIO_TALLER].anguloOff = 68;  // Angulo para DCC=off
  acc[DESVIO_TALLER].anguloOn = 93;   // Angulo para DCC=on
  acc[DESVIO_TALLER].output = false;
  acc[DESVIO_TALLER].servo.attach(SERVO_TALLER);

}  // END ConfigureFunctionsAndServos()

//////////////////////////////////////////////////////////////////////////////
// DCC packet handler
//////////////////////////////////////////////////////////////////////////////
void BasicAccDecoderPacket_Handler(int address, boolean activate, byte data) {
  // conversión de direcciones NMRA a un formato humano
  address -= 1;
  address *= 4;
  address += 1;
  address += (data & 0x06) >> 1;

  boolean enable = (data & 0x01) ? 1 : 0;
  Serial.print("Direccion de accesorio: ");
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
//////////////////////////////////////////////////////////////////////////////
// Setup (run once)
//////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  Serial.println(F("DECODER 1_1"));

  DCC.SetBasicAccessoryDecoderPacketHandler(BasicAccDecoderPacket_Handler, true);

  ConfigureFunctionsAndServos();

  for (int i = 0; i < MAX_ACC; i++) {
    acc[i].angulo = acc[i].anguloOff;  // Set start up angle to avoid movement at power on
    acc[i].servo.write(acc[i].angulo);
    pinMode(acc[i].outputPin0, OUTPUT);
    pinMode(acc[i].outputPin1, OUTPUT);

    digitalWrite(acc[i].outputPin0, HIGH);
    digitalWrite(acc[i].outputPin1, HIGH);
    delay(150);
    digitalWrite(acc[i].outputPin0, LOW);
    digitalWrite(acc[i].outputPin1, LOW);
    acc[i].servo.detach();
  }
  delay(1000);


  pinMode(P_FAROLAS, OUTPUT);
  pinMode(P_CASAS, OUTPUT);
  pinMode(P_LUCES_TALLER, OUTPUT);

  pinMode(SEMAFORO1_1, OUTPUT);
  pinMode(SEMAFORO1_2, OUTPUT);

/*
  digitalWrite(P_FAROLAS, HIGH);
  digitalWrite(P_CASAS, HIGH);
  digitalWrite(P_LUCES_TALLER, HIGH);
  digitalWrite(P_LUZ_CARRETERA, HIGH);
  digitalWrite(SEMAFORO1_1, HIGH);
  digitalWrite(SEMAFORO1_2, HIGH);
  delay(2000);
  digitalWrite(P_FAROLAS, LOW);
  digitalWrite(P_CASAS, LOW);
  digitalWrite(P_LUCES_TALLER, LOW);
  digitalWrite(P_LUZ_CARRETERA, LOW);
  digitalWrite(SEMAFORO1_1, LOW);
  digitalWrite(SEMAFORO1_2, LOW);

*/
  // Interrupt 0 with internal pull up resistor (can get rid of external 10k)
  // DCC.SetupDecoder(kCV_ManufacturerVersionNo, kCV_ManufacturedID, interrupt);
  DCC.SetupDecoder(0x00, 0x00, kDCC_INTERRUPT);
  pinMode(DCC_PIN, INPUT_PULLUP);  
  
  // Inicializa la librería NeoPixel.
  pixels.begin();            
} //END SETUP

//////////////////////////////////////////////////////////////////////////////
// Main loop (run continuous)
//////////////////////////////////////////////////////////////////////////////
void loop() {


  DCC.loop();  // DCC library

  Funciones();

  if (estadoLuces) luces_exteriores();


  if (millis() > timetoupdatesetpoint) {
    timetoupdatesetpoint = millis() + servotimer;
    for (int n = 0; n < MAX_ACC; n++) {

      if (acc[n].output) {
        acc[n].setpoint = acc[n].anguloOn;

      } else {
        acc[n].setpoint = acc[n].anguloOff;
      }

      if (acc[n].angulo < acc[n].setpoint) acc[n].angulo++;
      if (acc[n].angulo > acc[n].setpoint) acc[n].angulo--;
      acc[n].servo.write(acc[n].angulo);
      /*
        Serial.print("Servo ");
        Serial.print(n);
        Serial.print(" Posicion: ");
        Serial.println(acc[n].angulo);
        */


      if (acc[DESVIO_PRINCIPAL].angulo == acc[DESVIO_PRINCIPAL].setpoint) {  //  Si el angulo de giro es igual al de setpoint desactiva los servos
        acc[DESVIO_PRINCIPAL].servo.detach();                                //desactiva los servos
      } else {                                                               //  Si el angulo de giro es diferente al de setpoint activa los servos
        if (acc[DESVIO_PRINCIPAL].servo.attached() == false) acc[DESVIO_PRINCIPAL].servo.attach(SERVO_PRINCIPAL);

        // acc[DESVIO_PRINCIPAL].servo.attach(SERVO_PRINCIPAL);
      }
      if (acc[DESVIO_TALLER].angulo == acc[DESVIO_TALLER].setpoint) {  //  Si el angulo de giro es igual al de setpoint desactiva los servos
        acc[DESVIO_TALLER].servo.detach();
      } else {  //  Si el angulo de giro es diferente al de setpoint activa los servos
        Serial.println(acc[DESVIO_TALLER].angulo);
        if (acc[DESVIO_TALLER].servo.attached() == false) acc[DESVIO_TALLER].servo.attach(SERVO_TALLER);
      }
    }
  }
}  //END MAIN LOOP

void serial(String datosSeriales) {

  Serial.println(datosSeriales);
}
void mensaje(int i) {
  switch (i) {
    case ACC_LUCES_EXTERIOR:
      if (acc[ACC_LUCES_EXTERIOR].output) Serial.println(F("*-Luces Exteriores ON"));
      else Serial.println(F("*-Luces Exteriores OFF"));
      break;
    case ACC_LUCES_CASAS:
      if (acc[ACC_LUCES_CASAS].output) Serial.println(F("-*-Luces interiores ON"));
      else Serial.println(F("*-Luces interiores OFF"));
      break;
    case ACC_LUCES_TALLER:
      if (acc[ACC_LUCES_TALLER].output) Serial.println(F("*-Luces talleres ON"));
      else Serial.println(F("*-Luces talleres OFF"));
      break;
    case ACC_LUCES_CARRETERA:
      if (acc[ACC_LUCES_CARRETERA].output) Serial.println(F("*-Luces Carretera ON"));
      else Serial.println(F("*-Luces carretera OFF"));
      break;
    case DESVIO_TALLER:
      if (acc[DESVIO_TALLER].output) Serial.println(F("*-Semáforos Taller paso derecha"));
      else Serial.println(F("*-Semáforos Taller paso izquierda"));
      break;
    case ACC_EFECTO_TV:
      if (acc[ACC_EFECTO_TV].output) Serial.println(F("*-Efecto TV ON"));
      else Serial.println(F("*-Efecto TV OFF"));
      break;
  }
}

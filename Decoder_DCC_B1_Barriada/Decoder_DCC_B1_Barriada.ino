/* Modulo Bucle Retorno 1 - Barriada || 14-02-2025
//
//  --------------------------------------------------------
//  # Luces Exteriores               Dir=  1;  Subdir= 1;  F=2;
//  # Animación Casas Barriada       Dir=  1;  Subdir= 2;  F=3;
//  # Cambio Animación Barriada      Dir=  3;  Subdir= 0;  F=9;
//
//  --------------------------------------------------------

//////////////////////////////////////////////////////////////////
//Distribucion de pines:
// D02 = Entrada DCC
// D03 = LUCES CALLES
// D04 = HOME 1 ROOM 1
// D05 = HOME 1 ROOM 2
// D06 = HOME 2 ROOM 1
// D07 = HOME 2 ROOM 2
// D08 = HOME_3 ROOM 1
// D09 = HOME 3 ROOM 2
// D10 = HOME 4 ROOM 1
// D11 = HOME 4 ROOM 2
// D12 = HOME 4 ROOM 3
// D13 =
// A0 = 
// A1 =
// A2 =
// A3 =
*/
#include <DCC_Decoder.h>
#include "listaAccesorios.h"

#define kDCC_INTERRUPT  0   // Interrupción DCC
#define DCC_PIN         2   // Entrada señal DCC chip 6N137


// Nombre para los pines //
#define PIN_FAROLAS   3   // Luces Farolas (PWM)


// Definimos los modos
#define MODE_ALL_ON   1
#define MODE_ALEATORY 2
#define MODE_SECUENCY 3

int mode = MODE_ALEATORY;

int oldPin = 0;
bool funcionCasas = false;

// Secuencia de pines para el efecto secuencia (puedes modificarla a tu gusto)
int secuencia[] = {4, 6, 8, 10, 12, 10, 8, 6};


unsigned long timerModoAleatorio = 0;
unsigned long timerModoSecuencia = 0;

// Animacion Luces Progresivas
bool funcionExteriores = false;
bool estadoExteriores = false;
const int inicioLuces = 10;  // Intensidad inicial de las luces
const int finalLuces = 150;  // Intensidad final de las luces
int encendidoLuces = inicioLuces;
unsigned long previousMillisLuces = 0;  // Almacena la última actualización
const unsigned long periodoLuces = 40;

//////////////////////////////////////////////////
#define MAX_ACC 2   // Número máximo de accesorios


// Número indice accItem (acc)
#define ACC_LUCES_EXTERIOR 0
#define ACC_CHANGE_HOUSE_MODE 1

struct accItem {
  int direccion;    // DCC address to respond to
  boolean output;   // State of DCC accessory: 1=on, 0=off (ECoS: on=straight, off=turnout)
  byte outputPin0;  // Arduino output pin for additional function (not where servo is attached to)
  byte outputPin1;  // Arduino output pin for additional function (not where servo is attached to)
  byte angulo;
  byte setpoint;
  byte anguloOff;
  byte anguloOn;
  //Servo servo;
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
  acc[ACC_LUCES_EXTERIOR].outputPin0 = PIN_FAROLAS;
  acc[ACC_LUCES_EXTERIOR].output = false;
  
  ////////////////
  // Dirección 9; Cambia el modo encendido casas ; Dirección 3 Subdirección 0
  // 
  acc[ACC_CHANGE_HOUSE_MODE].direccion = EF_BARRIADA;
  acc[ACC_CHANGE_HOUSE_MODE].output = false;
  

}  // END ConfigureFunctionsAndServos()

//////////////////////////////////////////////////////////////////////////////
// DCC packet handler
//////////////////////////////////////////////////////////////////////////////
void BasicAccDecoderPacket_Handler(int address, boolean activate, byte data) {
  //Serial.println("Lectura de paquetes");
  // conversión de direcciones NMRA a un formato humano
  address -= 1;
  address *= 4;
  address += 1;
  address += (data & 0x06) >> 1;

  boolean enable = (data & 0x01) ? 1 : 0;
  //Serial.print("Direccion de accesorio: ");
  //Serial.println(address);
  for (int read = 0; read < 5; read++)
  {
    for (int i = 0; i < MAX_ACC; i++) {
      if (address == acc[i].direccion) {
        if (enable) acc[i].output = true;
        else acc[i].output = false;
        //mensaje(i);  // DEBUG ONLY
        break;
      }
    }
  }
}
void setup()
{

  Serial.begin(115200);
	
  	for (int salida = 3; salida <= 12; salida++)
	{
    
		pinMode(salida, OUTPUT);
	}
  
	for (int luz = 3; luz <= 12; luz++)
	{
		digitalWrite(luz, HIGH);
		delay(300);
		digitalWrite(luz, LOW);
	}  
  digitalWrite(3, LOW);

  DCC.SetBasicAccessoryDecoderPacketHandler(BasicAccDecoderPacket_Handler, true);
  ConfigureFunctionsAndServos();
  DCC.SetupDecoder(0x00, 0x00, kDCC_INTERRUPT);
  pinMode(DCC_PIN, INPUT_PULLUP);  
  Serial.println("Starting Barriada decoder...");
}

void loop()
{

  DCC.loop();   // DCC library
  Funciones();  // Funciones.ino
  if (estadoExteriores) luces_exteriores();
  if (acc[ACC_CHANGE_HOUSE_MODE].output) Lights_Mode_selection();

}




void mensaje(int option) 
{
  String msg = "";
  switch (option) 
  {
    case ACC_LUCES_EXTERIOR:
      if (acc[ACC_LUCES_EXTERIOR].output) msg = "*-Luces Exteriores ON";
      else msg = "*-Luces Exteriores OFF";
      break;
    case ACC_CHANGE_HOUSE_MODE:
      if (acc[ACC_CHANGE_HOUSE_MODE].output) msg = "*-Efecto luces aleatorio Casas ON";
      else msg = "*-Efecto luces Casas Aleatorio OFF";
      break;
  }
  Serial.println(msg);
}

/*
 * Este ejemplo muestra cómo usar la biblioteca como decodificador de accesorios DCC o decodificador de señalización DCC.
 * Responde tanto a los paquetes de control de desvíos DCC normales como a los paquetes de aspecto de señal DCC más recientes.
 * También puede imprimir todos los paquetes DCC descomentando la línea "#define NOTIFY_DCC_MSG".

 * Ejemplo modificado para el uso de servos y semáforos.
 * Descarga desde GitHub: 
 * Web: https://www.infotronikblog.com y http://lamaquetade.infotronikblog.com 
 * 
 */
#include <NmraDcc.h>
#include <Servo.h>


NmraDcc  Dcc ;
DCC_MSG  Packet ;

// Define the Arduino input Pin number for the DCC Signal 
#define DCC_PIN   2

// Número máximo de accesorios
#define MAX_ACC   2

// Timer para el cambio de angulo. Valor bajo -> mayor velocidad
const uint8_t servotimer = 60;  

#define DESVIO_1  1

struct accItem {
  uint16_t address;        // DCC address to respond to
  bool output;        // Estado del accesorio DCC
  // variables digitales uint8_t = 0-255; int8_t = -128 -- +127
  uint8_t outputPin0; // Arduino output pin for additional function (not where servo is attached to)
  uint8_t outputPin1; // Arduino output pin for additional function (not where servo is attached to)
  
  // variables servo
  int8_t servoPin;   // El pin que le asignamos al servo
  uint8_t angulo;      // Esta solo se utiliza cuando cambia el angulo del servo. 
  uint8_t anguloOn;    // Angulo para servo en posición abierta
  uint8_t anguloOff;   // Angulo para servo en posición cerrada
  uint8_t setPoint;    // Punto inicial del servo. anguloOn/anguloOff
  Servo servo;
};
accItem acc[MAX_ACC];

unsigned long timeToUpdateSetpoint = millis() + servotimer;
uint8_t nPosition = 0;
uint8_t nAcc = 0;

// Quita el comentario para ver el debug en el monitor serial.
// #define DEBUG
// Uncomment to print all DCC Packets
//#define NOTIFY_DCC_MSG
#ifdef  NOTIFY_DCC_MSG
void notifyDccMsg( DCC_MSG * Msg)
{
  Serial.print("notifyDccMsg: ") ;
  for(uint8_t i = 0; i < Msg->Size; i++)
  {
    Serial.print(Msg->Data[i], HEX);
    Serial.write(' ');
  }
  Serial.println();
}
#endif

/*
 * Esta función se llama siempre que se recibe un paquete de desvío DCC normal 
 * y estamos en modo de direccionamiento de placa
 */
 
void notifyDccAccTurnoutBoard( uint16_t BoardAddr, uint8_t OutputPair, uint8_t Direction, uint8_t OutputPower )
{
  #ifdef DEBUG
    Serial.print(F("notifyDccAccTurnoutBoard: ")) ;
    Serial.print(BoardAddr,DEC) ;
    Serial.print(',');
    Serial.print(OutputPair,DEC) ;
    Serial.print(',');
    Serial.print(Direction,DEC) ;
    Serial.print(',');
    Serial.println(OutputPower, HEX) ;
  #endif
}

// 
/*
 * Se llama siempre que se recibe un paquete de desvío DCC normal 
 * y estamos en modo de direccionamiento de salida.
 * Addr -        Por dirección de salida. 
 * Direction -   Dirección del desvío/accesorio. Tiene un valor de 0 o 1.
 * OutputPower - Salida On/Off. Tiene los siguientes valores:
 *               0 - Salida apagada.
 *               1 - Salida encendida.
 */
void notifyDccAccTurnoutOutput( uint16_t Addr, uint8_t Direction, uint8_t OutputPower )
{
  setAccessoryFromNmra(Addr, Direction);
}

// Esta función se llama siempre que se recibe un paquete de aspecto de señal DCC
void notifyDccSigOutputState( uint16_t Addr, uint8_t State)
{
  #ifdef DEBUG
    Serial.print(F("notifyDccSigOutputState: ")) ;
    Serial.print(Addr,DEC) ;
    Serial.print(',');
    Serial.println(State, HEX) ;
  #endif
}

void setup()
{

  Serial.begin(115200);
  delay(200);
  Serial.println("NMRA DCC Example 1");
  
  // Creamos los accesorios
  setupCreateAccessory();

  // Configuramos los pines.
  setupSetOutputsAccessory();

/* 
 * Configurar la interrupción externa, el pin al que está asociada y activar el pull-up.
 * Muchos núcleos Arduino ahora admiten la función digitalPinToInterrupt(), 
 * que facilita la determinación del número de interrupción para el pin de Arduino, lo que reduce la confusión.
 */
  #ifdef digitalPinToInterrupt
    Dcc.pin(DCC_PIN, 0);
  #else
    Dcc.pin(0, DCC_PIN, 1);
  #endif
  
  /* LLama a la función principal Init para activar el receptor DCC
  * MAN_ID_DIY = 0x0D;
  * CV29_ACCESSORY_DECODER = 0b10000000;    bit 7: "0" = Multi-Function Decoder Mode "1" = Accessory Decoder Mode
  * CV29_OUTPUT_ADDRESS_MODE = 0b01000000,  bit 6: "0" = Decoder Address Mode "1" = Output Address Mode
  * OpsModeAddressBaseCV  - Ops Mode base address. Set it to 0
  * Call the main DCC Init function to enable the DCC Receiver
  */
  Dcc.init( MAN_ID_DIY, 10, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE, 0 );
  
  Serial.println("Init Done");

}

void loop()
{
  // Se DEBE llamar al método NmraDcc.process() con frecuencia desde 
  // la función loop() de Arduino para el correcto funcionamiento de la biblioteca
  Dcc.process();
  
  runAccessory(nAcc);
  
  if (millis() > timeToUpdateSetpoint) {
    timeToUpdateSetpoint = millis() + servotimer;
    servoPoint();
  }
  nAcc++;
  if (nAcc == MAX_ACC) nAcc = 0;
}






/*
 * Llamada desde setup()
 * Asigna las variables de la estructura de datos
 * a cada accesorio.
 */

void setupCreateAccessory()
{
  acc[0].address =  5; 
  acc[0].output = false;
  acc[0].servoPin = -1;
  acc[0].outputPin0 = 4;
  acc[0].outputPin1 = 5;

  acc[1].address =  13;
  acc[1].output = false;
  acc[1].servoPin = 8;
  acc[1].outputPin0 = 6;
  acc[1].outputPin1 = 7;
  acc[1].anguloOn = 170;
  acc[1].anguloOff = 90;
  acc[1].setPoint = 170;
}

/*
 * Llamada desde setup()
 * Configura los pines de salida de cada accesorio
 * como opción también se testean las salidas.
 */
void setupSetOutputsAccessory()
{
  // Ponemos los pines del 3 al 8 como salidas.
  for (uint8_t pin = 4; pin <= 8; pin++ )
  {
    pinMode(pin, OUTPUT);
  }

  // Comprobamos que los pines encienden:
  // Opcional
  for (uint8_t nAcc = 0; nAcc < MAX_ACC; nAcc++)
  {
    digitalWrite(acc[nAcc].outputPin0, HIGH);
    digitalWrite(acc[nAcc].outputPin1, HIGH);
    delay(500);
    digitalWrite(acc[nAcc].outputPin0, LOW);
    digitalWrite(acc[nAcc].outputPin1, LOW);
    
    // si el accesorio tiene servo, inicia el servo.
    if (acc[nAcc].servoPin != -1) 
    {
      acc[nAcc].angulo = acc[nAcc].setPoint;
      acc[nAcc].servo.attach(acc[nAcc].servoPin);
    }
  }

}


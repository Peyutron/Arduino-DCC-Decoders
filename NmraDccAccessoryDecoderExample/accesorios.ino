
/*
 * Desde notifyDccAccTurnoutOutput escanea todos los accesorios buscando
 * coincidiencia en address
 * nAcc - Número de accesorio
 * state - 0=off 1=on
 */

void setAccessoryFromNmra(uint16_t nAcc, uint8_t state)
{

  for (int acc_ = 0; acc_ <= MAX_ACC -1; acc_++)
  {
    /* 
    * Si coinciden el número del accesorio recibido por la central
    * Con el número almacenado en address, cambia el estado Output
    */

    if (nAcc == acc[acc_].address)
    {
      if(state == 1) acc[acc_].output = true;
      else acc[acc_].output = false;
    } 
  }
  // #ifdef DEBUG
      Serial.print(F("setAccesory: "));
      Serial.print(nAcc);
      Serial.print(F(", State:"));
      Serial.print(state);
  // #endif

}

/*
 * Ejecuta las salidas de los accesorios como semáforos o luces.
 * nAcc - valor para iterar los accesorios
 */

void runAccessory(uint8_t nAcc)
{
  if (acc[nAcc].output)
  {
    digitalWrite(acc[nAcc].outputPin0, HIGH);
    digitalWrite(acc[nAcc].outputPin1, LOW);
  }
  else 
  {
    digitalWrite(acc[nAcc].outputPin0, LOW);
    digitalWrite(acc[nAcc].outputPin1, HIGH);
  }
}

/* 
 * Posición del servo que se actualiza desde el loop()
 * con los pasos de actualización con la variable "servotimer = 60"
 * Si servotimer es grande el servo irá mas lento.
 * Si servotimer es pequeño el servo irá más rápido. 
 */

void servoPoint()
{

  if (acc[nPosition].servoPin != -1)
  { 
    // output=true-anguloOn output=false anguloOff
    if (acc[nPosition].output) 
    {
      acc[nPosition].setPoint = acc[nPosition].anguloOn;
    } 
    else 
    {
      acc[nPosition].setPoint = acc[nPosition].anguloOff;  
    }

    if (acc[nPosition].angulo < acc[nPosition].setPoint) acc[nPosition].angulo++;
    if (acc[nPosition].angulo > acc[nPosition].setPoint) acc[nPosition].angulo--;
    acc[nPosition].servo.write(acc[nPosition].angulo);
    
    #ifdef DEBUG
        Serial.print(F(" Servo "));
        Serial.print(nPosition);
        Serial.print(F(" Posicion: "));
        Serial.println(acc[nPosition].angulo);
    #endif

    if (acc[nPosition].angulo == acc[nPosition].setPoint) 
    {  
      //  Si el angulo de giro es igual al de setpoint desactiva los servos
      acc[nPosition].servo.detach();   // Desactiva los servos
    } 
    else 
    {
      //  Si el angulo de giro es diferente al de setpoint activa los servos
      if (acc[nPosition].servo.attached() == false) acc[nPosition].servo.attach(acc[nPosition].servoPin);
    }
  }   
  // Siguiete posición;
  // Serial.print(nPosition);
  nPosition++;
  if (nPosition == MAX_ACC) nPosition = 0;
  
}
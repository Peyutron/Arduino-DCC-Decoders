void Ocupacion() 
{
  for (int Nsensor = 0; Nsensor < MAX_SENSORES; Nsensor++) 
  {
    sensor[Nsensor].estado = digitalRead(sensor[Nsensor].pin);
  }    

  // sensor[0].pin);   // E/S bucle sur
  // sensor[1].pin);   // Interior bucle sur
  // sensor[2].pin);   // E/S bucle norte
  // sensor[2].pin);   // Interior bucle norte

  sensorCheckStatus();
  sensorRelayStatus();
}


void sensorCheckSpeed()
{

    if (digitalRead(sensor[0].pin) == 1)
    {
      if (startTime == 0) 
      {
        startTime = millis();
        Serial.println(startTime);
      }
    }

    if (digitalRead(sensor[1].pin) == 1)
    {
      if (endTime == 0)
      {
        endTime = millis();
        Serial.println(endTime);
      }
    }
  if (!executed)
  {
    // Inicia el calculo:
    if (startTime != 0 && endTime != 0)
    {
      unsigned long diferencia = (startTime - endTime);
      if (diferencia > 200)
      {
        float velocidad = (DISTANCIA * 10000000.0) / diferencia;
        float scaleSpeed = velocidad * 0.036;
        showSpeed3x7(scaleSpeed);
        Serial.print(F("Velocidad Km/h: "));
        Serial.println(scaleSpeed,1);
        executed = false;
        startTime = 0;
        endTime = 0;
      }
    }
  }
}

// Muestra la velocidad en la pantalla 3x7Segmentos
void showSpeed3x7(float scaleSpeed)
{
  int speed;
  if (scaleSpeed < 99)
  {
    speed = round(scaleSpeed * 10);
    display.displayInteger(speed);
    display.displayDP(true);
  }
  else
  {
    display.displayInteger(scaleSpeed);
    display.displayDP(false);
  }
}

// Comprueba el estado de los sensores
void sensorCheckStatus()
{
  // for (int i = SENS10; i <= SENS13; i++) 
  for (int i = 0; i < MAX_SENSORES; i++)
  {
    if (sensor[i].estado && sensor[i].old_estado) 
    {
      sensor[i].old_estado = false;
      sendBloqueRF(sensor[i].id, sensor[i].estado);
    }
    
    if (!sensor[i].estado && !sensor[i].old_estado)
    { 
      sensor[i].old_estado = true;
      sendBloqueRF(sensor[i].id, sensor[i].estado);
    }
  }
}

// Comprueba el estado del rele
void sensorRelayStatus()
{
  // ESTADO RELE LOOP
  if ((sensor[0].estado || sensor[2].estado) && statusrelay) 
  {
    statusrelay = !statusrelay;
    digitalWrite(RELELOOP, LOW);
    Serial.println(F("DETECTOR IR E/S NORTE LOW"));
  }
  if ((sensor[1].estado || sensor[3].estado) && !statusrelay) 
  {
    statusrelay = !statusrelay;
    digitalWrite(RELELOOP, HIGH);
    Serial.println(F("DETECTOR IR NORTE INTERIOR HIGH"));
  }

  /*
  if (bloque[2] && statusrelay) {
    statusrelay = !statusrelay;
    digitalWrite(RELELOOP, LOW);
    Serial.println(F("DETECTOR IR E/S SUR LOW"));
  }

  if (bloque[3] && !statusrelay) {
    statusrelay = !statusrelay;
    digitalWrite(RELELOOP, HIGH);
    Serial.println(F("DETECTOR IR SUR INTERIOR HIGH"));
  }
  */
}




void Ocupacion() 
{
  //Serial.println("Ocupacion");
  for (int Nsensor = 0; Nsensor < MAX_SENSORES; Nsensor++) 
  {
    sensor[Nsensor].estado = digitalRead(sensor[Nsensor].pin);   // E/S bucle sur
    //Serial.println(sensor[Nsensor].estado);
  }    

  sensorCheckStatus();
  sensorRelayStatus();
}

void sensorCheckStatus()
{
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

void sensorRelayStatus()
{
  if (!sensor[SENS03].estado) digitalWrite(RELELOOP, LOW);
  else if (!sensor[SENS05].estado) digitalWrite(RELELOOP, HIGH);
  else if (sensor[SENS01].estado) digitalWrite(RELELOOP, LOW);
  else if (sensor[SENS02].estado) digitalWrite(RELELOOP, HIGH);
}

void Funciones() 
{
  ////////////////////////////////////////////////
  // Establece la salida para las luces exteriores
  if (acc[ACC_LUCES_EXTERIOR].output) 
  {
    luces_exteriores();
  } 
  else 
  {
    funcion_luces_inicio = true;       // Secuencia encendido 1/3
    funcion_luces_progresivo = false;  // Secuencia progresiva 2/3
    funcion_luces_encendidas = false;  // Secuencia encendido 3/3
    digitalWrite(acc[ACC_LUCES_EXTERIOR].outputPin0, LOW); // Apagamos la salida de luces exteriores
  }

  ////////////////////////////////////////////////
  // Establece la salida para las luces interiores
  if (acc[ACC_LUCES_INTERIOR].output) digitalWrite(acc[ACC_LUCES_INTERIOR].outputPin0, HIGH);
  else digitalWrite(acc[ACC_LUCES_INTERIOR].outputPin0, LOW);

  /////////////////////////////////////
  // Establece la salida semáforo tunel
  if (acc[SIGNAL_SEMAFORO_TUNEL].output) 
  {
    digitalWrite(acc[SIGNAL_SEMAFORO_TUNEL].outputPin0, HIGH);
    digitalWrite(acc[SIGNAL_SEMAFORO_TUNEL].outputPin1, LOW);
  } 
  else 
  {
    digitalWrite(acc[SIGNAL_SEMAFORO_TUNEL].outputPin0, LOW);
    digitalWrite(acc[SIGNAL_SEMAFORO_TUNEL].outputPin1, HIGH);
  }
}

////////////////////////////////////
// Función para las luces exteriores
void luces_exteriores()
{
  // Inicia la animación encendiendo y apagando las luces
  if (funcion_luces_inicio) 
  {
    bool status = true;
    for (int repeticion = 0; repeticion < 3; repeticion++)
    {
      analogWrite(acc[ACC_LUCES_EXTERIOR].outputPin0, 80);  // PWM
      delay(100);
      digitalWrite(acc[ACC_LUCES_EXTERIOR].outputPin0, LOW);
      delay(100);
    }
    funcion_luces_inicio = false;
    funcion_luces_progresivo = true;
  }  // FIN primera parte encendido

  // Segunda parte animación encendido progresivo
  if (funcion_luces_progresivo) 
  {
    unsigned long currentMillisLuces = millis();
    if (currentMillisLuces - previousMillisLuces >= periodoLuces)
    {
      encendidoLuces++;
      analogWrite(acc[ACC_LUCES_EXTERIOR].outputPin0, encendidoLuces);
      if (encendidoLuces == final_luces_pwm)
      {
        Serial.println(F("FINAL LUCES"));
        encendidoLuces = inicia_luces_pwm;
        funcion_luces_progresivo = false;
        funcion_luces_encendidas = true;
      }
      previousMillisLuces = currentMillisLuces;
    }
  }  // FIN segunda parte encendido

  // Tercera parte animación luces encendidas
  if (funcion_luces_encendidas) analogWrite(acc[ACC_LUCES_EXTERIOR].outputPin0, final_luces_pwm);
}

 
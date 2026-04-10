


void Funciones() 
{
  
  ///////////////////////////////////////////////
  // Establece la salida para las luces exteriores
  if (acc[ACC_LUCES_EXTERIOR].output && !funcionExteriores)
  {
    // Serial.println("Exterior HIGH");
    analogWrite(acc[ACC_LUCES_EXTERIOR].outputPin0, 100);
    delay(70);
    digitalWrite(acc[ACC_LUCES_EXTERIOR].outputPin0, LOW);
    delay(70);
    analogWrite(acc[ACC_LUCES_EXTERIOR].outputPin0, 100);
    delay(70);
    digitalWrite(acc[ACC_LUCES_EXTERIOR].outputPin0, LOW);
    funcionExteriores = true;
    estadoExteriores = true;
  } 
  else if (!acc[ACC_LUCES_EXTERIOR].output && funcionExteriores)
  {
    //Serial.println("Exterior LOW");
    analogWrite(acc[ACC_LUCES_EXTERIOR].outputPin0, LOW);
    funcionExteriores = false;
  }
  ///////////////////////////////////////////////
  // Establece la salida para iluminación de casas
  if (acc[ACC_CHANGE_HOUSE_MODE].output && !funcionCasas) 
  { 
    // Serial.println("Casas for HIGH");
    funcionCasas = true;
  }
  else if (!acc[ACC_CHANGE_HOUSE_MODE].output && funcionCasas) 
  {
    // Serial.println("Casas LOW");
    for (int pin = 4; pin <= 12; pin++) digitalWrite(pin, LOW);
    funcionCasas = false;
  }
  
}

void luces_exteriores() 
{
  unsigned long currentMillisLuces = millis();
  if (currentMillisLuces - previousMillisLuces >= periodoLuces) 
  {
    encendidoLuces++;
    analogWrite(acc[ACC_LUCES_EXTERIOR].outputPin0, encendidoLuces);
    if (encendidoLuces == finalLuces) 
    {
      // Serial.println("FINAL LUCES");
      encendidoLuces = inicioLuces;
      estadoExteriores = false;
    }
    previousMillisLuces = currentMillisLuces;
  }
}

void Lights_Mode_selection()
{
  if (mode == MODE_ALL_ON) Modo_On_Off();
	else if (mode == MODE_ALEATORY) Modo_Aleatorio();
	else if (mode == MODE_SECUENCY) Modo_Secuencia();
}

// Modo 1: Enciende y apaga LEDs 
void Modo_On_Off() 
{ 
  // Enciende o apaga todos los LEDs
  for (int pin = 4; pin <= 12; pin++) digitalWrite(pin, HIGH);
}

// Modo 2: Enciende y apaga LEDs de manera aleatoria
void Modo_Aleatorio() 
{
  const unsigned long intervalo = 5000; // Intervalo de 5 segundos
  //const unsigned long intervalo = 1000; // Intervalo de 1 segundos
  // Verifica si ha pasado el tiempo suficiente
  if (millis() - timerModoAleatorio >= intervalo) 
  
  {
    timerModoAleatorio = millis(); // Actualiza el tiempo anterior

    int pinAleatorio = random(4, 13); // Genera un número aleatorio entre 4 y 12
    while (pinAleatorio == oldPin)
    {
      pinAleatorio = random(4, 13); // Genera un número aleatorio entre 4 y 12
    } 

    oldPin = pinAleatorio;
    
    bool estado = random(2); // Genera 0 (apagado) o 1 (encendido)
    
    
    Serial.print("PIN: ");
    Serial.print(pinAleatorio);
    Serial.print(" Estado ");
    Serial.println(estado);
    Serial.print(" oldpin ");
    Serial.println(estado);
    

    digitalWrite(pinAleatorio, estado ? HIGH : LOW); // Enciende o apaga el LED aleatorio
  }
}

// Mode 3: Secuencia predeterminada
void Modo_Secuencia() 
{
  static int indiceSecuencia = 0; // Índice de la secuencia actual
  const unsigned long intervalo = 3000; // Intervalo de 20 seg 

  // Verifica si ha pasado el tiempo suficiente
  if (millis() - timerModoSecuencia >= intervalo) 
  {
    timerModoSecuencia = millis(); // Actualiza el tiempo anterior

    // Apaga todos los LEDs antes de encender el siguiente
    for (int pin = 4; pin <= 12; pin++) 
    {
      Serial.print("todos los pines a 0: ");
      Serial.println(pin);
      digitalWrite(pin, LOW);
    }

    // Enciende el LED actual en la secuencia
    digitalWrite(secuencia[indiceSecuencia], HIGH);

    // Avanza al siguiente paso en la secuencia
    indiceSecuencia = (indiceSecuencia + 1) % (sizeof(secuencia) / sizeof(secuencia[0]));
  }
}

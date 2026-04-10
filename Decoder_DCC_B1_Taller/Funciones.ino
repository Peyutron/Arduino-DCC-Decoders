
void Funciones() {
  ////////////////////////////////////////////////////////////////////////
  //Establece la salida para las luces exteriores
  if (acc[ACC_LUCES_EXTERIOR].output) {
    if (funcionLuces) {
      analogWrite(acc[ACC_LUCES_EXTERIOR].outputPin0, 100);
      delay(70);
      digitalWrite(acc[ACC_LUCES_EXTERIOR].outputPin0, LOW);
      delay(70);
      analogWrite(acc[ACC_LUCES_EXTERIOR].outputPin0, 100);
      delay(70);
      digitalWrite(acc[ACC_LUCES_EXTERIOR].outputPin0, LOW);
      funcionLuces = false;
      estadoLuces = true;
    }
  } else {
    analogWrite(acc[ACC_LUCES_EXTERIOR].outputPin0, LOW);
    funcionLuces = true;
    estadoLuces = false;
  }

  ///////////////////////////////////////////////////////////////////////
  //Establece la salida para las luces interiores
  if (acc[ACC_LUCES_CASAS].output)
  {
     analogWrite(acc[ACC_LUCES_CASAS].outputPin0, 50);
     analogWrite(acc[ACC_LUCES_CASAS].outputPin1, 255);
     
  }
  else 
  {
    digitalWrite(acc[ACC_LUCES_CASAS].outputPin0, LOW);
    digitalWrite(acc[ACC_LUCES_CASAS].outputPin1, LOW);
  }
  /////////////////////////////////////////////
  //Establece la salida para las luces talleres
  if (acc[ACC_LUCES_TALLER].output) analogWrite(acc[ACC_LUCES_TALLER].outputPin0, 60);
  else digitalWrite(acc[ACC_LUCES_TALLER].outputPin0, LOW);
  
  ///////////////////////////////////////////////
  // Establece la salida para las luces carreteras
  // if (acc[ACC_LUCES_CARRETERA].output) digitalWrite(acc[ACC_LUCES_CARRETERA].outputPin0, HIGH);
  // else digitalWrite(acc[ACC_LUCES_CARRETERA].outputPin0, LOW);
  ///////////////////////////////////////////////
  // Establece la salida para las luces exteriores
  if (acc[DESVIO_TALLER].output) {
    digitalWrite(acc[DESVIO_TALLER].outputPin0, HIGH);
    digitalWrite(acc[DESVIO_TALLER].outputPin1, LOW);
  } else {
    digitalWrite(acc[DESVIO_TALLER].outputPin0, LOW);
    digitalWrite(acc[DESVIO_TALLER].outputPin1, HIGH);
  }

  if (acc[ACC_EFECTO_TV].output) {
    Neopixel();
    estadoTV = true;
  } 
  else 
  {
    if (estadoTV) {
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));  // Moderately bright green color.
      pixels.show();                                   // This sends the updated pixel color to the hardware.
      estadoTV = false;
      // Serial.println("TELEVISOR APAGADO....");
    }
  }
}

void Neopixel() {
  unsigned long currentMillisTV = millis();
  if (currentMillisTV - previousMillisTV >= periodoTV) {

    pixels.setPixelColor(0, pixels.Color(delayval(), delayval(), delayval()));  // Moderately bright green color.
    pixels.show();                                                              // This sends the updated pixel color to the hardware.
    previousMillisTV = currentMillisTV;
  }
}

int delayval() {
  return random(500, 2000);
}

void luces_exteriores() {

  unsigned long currentMillisLuces = millis();
  if (currentMillisLuces - previousMillisLuces >= periodoLuces) {
    encendidoLuces++;
    analogWrite(acc[ACC_LUCES_EXTERIOR].outputPin0, encendidoLuces);
    if (encendidoLuces == finalLuces) {
      Serial.println("FINAL LUCES");
      estadoLuces = false;
      encendidoLuces = inicioLuces;
    }

    previousMillisLuces = currentMillisLuces;
  }
}

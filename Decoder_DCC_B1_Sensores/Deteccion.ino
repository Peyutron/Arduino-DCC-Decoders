void Ocupacion() {

  //// Sensores infrarrojos
  /////////////////////////
  bloque[SENS01] = digitalRead(SENSOR_IR_A);
  bloque[SENS02] = digitalRead(SENSOR_IR_B);

  //// Sensores de consumo
  ////////////////////////
  bloque[SENS03] = digitalRead(BLOQUE1PIN);
  bloque[SENS04] = digitalRead(BLOQUE2PIN);  // Interior Bucle
  bloque[SENS05] = digitalRead(BLOQUE3PIN);
  // bloque[SENS06] = digitalRead(BLOQUE4PIN);


  for (int i = SENS01; i <= SENS05; i++) {
    if (bloque[i] && BloqueStatus[i]) {
      BloqueStatus[i] = false;
      SendBloque(i, BloqueStatus[i]);
    }
    if (!bloque[i] && !BloqueStatus[i]) {
      BloqueStatus[i] = true;
      SendBloque(i, BloqueStatus[i]);
    }
  }

  // ESTADO RELE LOOP
  ///////////////////
  if (bloque[SENS01] && statusrelay) {
    statusrelay = !statusrelay;
    digitalWrite(RELELOOP, LOW);
    Serial.println(F("DETECTOR FRENTE"));
  }
  if (bloque[SENS02] && !statusrelay) {
    statusrelay = !statusrelay;
    digitalWrite(RELELOOP, HIGH);
    Serial.println(F("DETECTOR PARED"));
  }

  if (!bloque[SENS03] && statusrelay) {
    statusrelay = !statusrelay;
    digitalWrite(RELELOOP, LOW);
    Serial.println(F("DETECTOR FRENTE"));
  }
  if (!bloque[SENS05] && !statusrelay) {
    statusrelay = !statusrelay;
    digitalWrite(RELELOOP, HIGH);
    Serial.println(F("DETECTOR PARED"));
  }
}
void SendBloque(uint8_t sensor, uint8_t estado) {
  Serial.print(F("bloque "));
  Serial.print(sensor);
  Serial.print(F(" estado "));
  Serial.println(estado);


  char buf[VW_MAX_MESSAGE_LEN];  // Logitud del buffer 80 caracteres
  String str = "";

  if (estado) str = "<Q" + String(sensor) + ">";
  else str = "<q" + String(sensor) + ">";
  Serial.println(str);
  str.toCharArray(buf, sizeof(buf));                                 // Convertimos el String en un array
  for (int x = 0; x < 4; x++) vw_send((uint8_t *)buf, strlen(buf));  // Enviamos el array
  vw_wait_tx();                                                      // Esperamos hasta que el mensaje se envie
}

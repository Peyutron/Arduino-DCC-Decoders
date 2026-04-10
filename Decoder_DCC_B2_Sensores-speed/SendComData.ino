/* DEBUG
void printMessage(int option) 
{
  String msg = "";
  if (option == ACC_LUCES_EXTERIOR) 
  {
    if (acc[ACC_LUCES_EXTERIOR].output) msg = "*-Luces Exteriores ON";
    else msg = "*-Luces Exteriores OFF";
  }
  else if (option ==  ACC_LUCES_INTERIOR)
  {
    if (acc[ACC_LUCES_INTERIOR].output) msg = "*-Luces interiores ON";
    else msg = "*-Luces interiores OFF";
  }
  Serial.println(msg);
}
*/

// Envía los datos por el módulo FS1000A RF433
void sendBloqueRF(uint8_t sensor, uint8_t estado) 
{
  // Serial.print(F("bloque "));
  // Serial.print(sensor);
  // Serial.print(F(" estado "));
  // Serial.println(estado);

  char buf[VW_MAX_MESSAGE_LEN];  // Logitud del buffer 80 caracteres
  String str = "";

  if (estado) str = "<Q" + String(sensor) + ">";
  else str = "<q" + String(sensor) + ">";
  Serial.println(str);
  str.toCharArray(buf, sizeof(buf));                                 // Convertimos el String en un array
  for (int x = 0; x < 4; x++) vw_send((uint8_t *)buf, strlen(buf));  // Enviamos el array
  vw_wait_tx();                                                      // Esperamos hasta que el mensaje se envie
}
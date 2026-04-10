Estos son los Decodificadore DCC que utilizo en la http://lamaquetade.infotronikblog.com<br>
Arduino NANO

* Librerías externas:
  - **DCC_Decoder** https://github.com/MynaBay/DCC_Decoder
  - **TM1651** https://github.com/ilneill/Gotek-LEDC68
  - **Adafruit_NeoPixel** https://github.com/adafruit/Adafruit_NeoPixel

* Decoder_DCC_B1_Barriada:
   - Distribucion de pines:
   - D02 = Entrada DCC
   - D03 = Luces farolas (PWM)
   - D04 = HOME 1 ROOM 1
   - D05 = HOME 1 ROOM 2
   - D06 = HOME 2 ROOM 1
   - D07 = HOME 2 ROOM 2
   - D08 = HOME_3 ROOM 1
   - D09 = HOME 3 ROOM 2
   - D10 = HOME 4 ROOM 1
   - D11 = HOME 4 ROOM 2
   - D12 = HOME 4 ROOM 3
   - D13 =
   - A0 =
   - A1 =
   - A2 =
   - A3 =

* Decoder_DCC_B1_Sensores:
     - Distribucion de pines:
     - D2 = Entrada DCC
     - D3 = Sensor IR Loop A
     - D4 = Sensor IR Loop B
     - D5 = Luces farolas (PWM)
     - D6 = Luces interior casas (PWM)
     - D7 = Pin Tx RF433  vw_tx_pin = 7 (módulo FS1000A)
     - D8 = Luz semáforo - rojo
     - D9 = Luz semáforo - verde
     - D10 = Rele corazón desvío cocheras
     - D11 = Relé Bucle de retorno
     - D12 = Soldador
     - D13 =
     - A0 = Sensor Bloque 1 E/S Sur SENS03
     - A1 = Sensor Bloque 2 Interior bucle SENS04
     - A2 = Sensor Bloque 3 E/S Norte SENS05
     - A3 = Sensor Bloque 4 NO USADO

* Decoder_DCC_B1_Taller:
     - Distribucion de pines:
     - D2 = Entrada DCC
     - D3 = Luces farolas talleres
     - D4 = NEOPIXEL (TV)
     - D5 = Luces farolas (PWM)
     - D6 = LUCES INTERIORES
     - D7 = Servo Desvío Talleres
     - D8 = Servo Desvío Principal
     - D9 = LUCES CARRETERA
     - D10 = SEMAFORO 1_1
     - D11 = SEMAFORO 1_2
     - D12 =
     - D13 =
     - A0 =
     - A1 =
     - A2 =
     - A3 =

* Decoder_DCC_B2_Sensores:
     - Distribucion de pines:
     - D2 = Entrada DCC
     - D3 =
     - D4 =
     - D5 = Luces farolas (PWM)
     - D6 = Luces interior casas (PWM)
     - D7 = Pin Rx RF433  vw_tx_pin = 7 (módulo FS1000A)
     - D8 =
     - D9 =
     - D10 =
     - D11 =
     - D12 =
     - D13 =
     - A0 = Sensor IR loop Bloque 6 E/S Sur SENS06
     - A1 = Sensor IR loop Bloque 7 E/S Sur SENS07
     - A2 = Sensor IR loop Bloque 8 E/S Norte SENS08
     - A3 = Sensor IR loop Bloque 9 E/S Norte SENS09
     - A4 = CLKPIN 3x7Segments
     - A5 = DIOPIN 3x7Segments




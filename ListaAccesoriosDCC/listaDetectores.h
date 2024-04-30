// LISTA DE SENSORES:

// Modulo bucle retorno 1:

// Sensores
#define SENSORES_MAX 10

#define SENS01		1	// B1 -> Sensor IR Norte Interior bucle
#define SENS02		2	// B1 -> Sensor IR Sur   Interior bucle
#define SENS03		3	// B1 -> Sensor Consumo E/S Sur
#define SENS04		4	// B1 -> Sensor Consumo Interior bucle
#define SENS05		5	// B1 -> Sensor Consumo E/S Norte
#define SENS06		6 	// No Usado
#define SENS07		7 	// No Usado
#define SENS08		8 	// No Usado
#define SENS09		9 	// No Usado

/// NUMERO DE ACCESORIO 



// LISTA DE DESVIOS: 

// Modulo bucle retorno 1:

//  Modulo Bucle Retorno 1 || 05-09-2024
//  #Desvio 1   Entrada cocheras      Dir=  5;  Subdir= 0;  F=17;
//  #Desvio 2   Cocheras              Dir=  5;  Subdir= 1;  F=18;
//  --------------------------------------------------------
//  #Animacion soldador               Dir=  1;  Subdir= 1;  F=2;
//  #Luces Exteriores                 Dir=  1;  Subdir= 2;  F=3;
//  #Luces Casas                      Dir=  1;  Subdir= 3;  F=4;
//  --------------------------------------------------------
//  #Dirección I2C = 1

// Modulo 2:

//  Modulo 02 || 27-04-2020 Caseta de agujas
//  #Desvio Sur-Este Estación 1:      Dir=  5;  Subdir= 2;  F=19;
//  #Desvio Sur-Este Apeadero 1:      Dir=  5;  Subdir= 3;  F=20;
//  --------------------------------------------------------
//  #Luces Exteriores                 Dir=  1;  Subdir= 2;  F=3;
//  #Luces Casas                      Dir=  1;  Subdir= 3;  F=4;

// Modulo 3:

// Modulo 03 || 23-10-2018
//  #Desvio Sur-Oeste Estación 1:     Dir=  6;  Subdir= 0;  F=21;
//  --------------------------------------------------------
//  #Luces Exteriores                 Dir=  1;  Subdir= 2;  F=3;
//  #Luces Casas                      Dir=  1;  Subdir= 3;  F=4;
//  #Animacion: Paso a nivel          Dir=  2;  Subdir= 0;  F=5;  ****

// Modulo 4:

//  Modulo 04 || 23-10-2018
//  #Desvio Norte-Oeste Estación 3:   Dir=  6;  Subdir= 1;  F=22;
//  #Desvio Norte-Oeste Cocheras:     Dir=  6;  Subdir= 2;  F=23;
//  #Cruce  Norte-Oeste               Dir=  6;  Subdir= 3;  F=24;
//  #Desvio Norte-Oeste Estación 2    Dir=  7;  Subdir= 0;  F=25;
//  --------------------------------------------------------
//  #Luces Exteriores                 Dir=  1;  Subdir= 2;  F=3;
//  #Luces Casas                      Dir=  1;  Subdir= 3;  F=4;
//  #Animaciones                      Dir=  2;  Subdir= 0;  F=5;  ****

// Modulo 5:

//  Modulo 05 || 23-10-2018
//  # Desvio Norte-Este Estación 2:   Dir=  7;  Subdir= 1;  F=26;
//  # Desvio Norte-Este Estación 3:   Dir=  7;  Subdir= 2;  F=27;
//  # Cruce  Norte-Este               Dir=  7;  Subdir= 3;  F=28;
//  --------------------------------------------------------
//  # Luces Exteriores                Dir=  1;  Subdir= 2;  F=3;
//  # Luces Casas                     Dir=  1;  Subdir= 3;  F=4;


// Modulo bucle de retorno 2:

//Listado Locomotoras y decoders:

/*
*- Talgo 352-005-3            *-Ibertren      *-LaisDCC             *-2108      *-Cv1= 8;Cv2=;  *-Extra: Motor CDrom
*- Renfe 10702                *-Roco          *-Lenz Digital plus   *-63924     *-Cv1= 2;Cv2=;  *-Extra: Valenciana
*- Renfe D319.339.8           *-Roco          *-LaisDCC             *-63968     *-Cv1= 7;Cv2=;  *-Extra: Mant. Infraestructura
*- AVE 100                    *-Mehano        *-NCE                 *-          *-Cv1= 2;Cv2=;  
*- DR 132 577-8               *-Roco          *-LaisDCC             *-63431     *-Cv1= 5;Cv2=;
*- Alco 333-084               *-Electrotren   *-Arduino             *-          *-Cv1=10;Cv2=;
*- Renfe Mitsubishi 269-285-3 *-Ibertren      *-                    *-2116      *-Cv1=  ;Cv2=;  *-Extra:      

*/

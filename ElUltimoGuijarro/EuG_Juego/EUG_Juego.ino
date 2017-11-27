/*

 *     ********************************************************
 *     ********************************************************
 *     ***                                                  ***
 *     ***               Artificial Inteligence             ***
 *     ***                                                  ***
 *     ********************************************************
 *     ********************************************************

 *   ****************************************************
 *   * Fecha: 29/12/2016                                *
 *   * Autor: Ruben Fernandez Pacheco                   *
 *   * Mail: r.zordke@hotmail.com                       *
 *   * Licencia: GNU General Public License v3 or later *
 *   ****************************************************
*/

/******************************************************************
                              Librerias
 ******************************************************************/
#include "Arduino.h"
#include <EEPROMex.h>

/******************************************************************
                      Definicion de variables
 ******************************************************************/
#define Jugador1  0
#define Jugador2  1

/* variables usadas para el uso de la EEPROM */
int address;
const int maxAllowedWrites = 80;
const int memBase          = 350;

/* variables usadas para el comportamiento */
int anteriorGuijarros[2], guijarros, anteriorEleccion[2], eleccion, contador;
bool anteriorValido[2];

/******************************************************************
                             Funciones
 ******************************************************************/
int comprobarBit(int guijarro, int cogidos)
{
  int posicion = (guijarro - 1) * 3 + (cogidos - 1);
  return bitRead( EEPROM.read(address + posicion / 8)  , posicion % 8 );
}

void invalidarBit(int guijarro, int cogidos)
{
  byte base = B00000001, output;
  int posicion = (guijarro - 1) * 3 + (cogidos - 1);
  base = ~(base << posicion % 8);
  output = EEPROM.read(address + posicion / 8);
  EEPROM.write(address + posicion / 8, output & base);
  delay(70);
}

int juegaPc(int guijarro, int jugador) {
  int opciones = 0, resultado;
  byte output;

  //Primero miro las opciones que tengo validas para este numero de guijarros
  for (unsigned int i = 1; i <= 3 ; i++)
  {
    if (comprobarBit(guijarro, i))
    {
      opciones++;
    }
  }

  //Si tengo ocpciones validas eligo una al azar
  if (opciones > 0)
  {
    //Compruebo que he elegido al azar una opcion valida
    do {
      eleccion = random(1, 4);
    } while (!comprobarBit(guijarro, eleccion));
    //Sustraigo el numero de guijarros elegidos
    resultado = guijarros - eleccion;
    //Si me queda menos guijarros he perdido con esa elección por lo que escribire que esa elección
    //es invalida
    if (resultado < 1)
    {
      invalidarBit(guijarro, eleccion);
    }else{ //sino es valida
      anteriorValido[jugador] = true;
    }
  } else {;
    //Invalido mi anterior elección si mi eleccion anterior es valida
    if (anteriorValido[jugador])
    {
      invalidarBit(anteriorGuijarros[jugador], anteriorEleccion[jugador]);
    }
    anteriorValido[jugador] = false;
    //Eligo una al azar de entre mis opciones invalidas
    eleccion = random(1, 4);
    resultado = guijarro - eleccion;
  }
  //Devuelvo cuantos guijarros quedan y me guardo mi eleccion
  anteriorGuijarros[jugador] = guijarro;
  anteriorEleccion[jugador] = eleccion;
  return resultado;
}

void mostrarMatriz()
{
  for (unsigned int i = 1 ; i <= 21 ; i++)
  {
    for (unsigned int j = 1; j <= 3 ; j++)
    {
      Serial.print("Si tengo: ");
      Serial.print(i);
      Serial.print(" y cojo : ");
      Serial.print( j );
      Serial.print(" mi jugada es ");
      if ( comprobarBit(i, j) )
      {
        Serial.println(" valida ");
      } else {
        Serial.println(" no valida ");
      }
    }
  }
  Serial.println("************************************************");
}

/******************************************************************
                               Setup
 ******************************************************************/
void setup() {
  // Empezamos la conexión serie
  Serial.begin(9600);

  // start reading from position memBase (address 0) of the EEPROM. Set maximumSize to EEPROMSizeUno
  // Writes before membase or beyond EEPROMSizeUno will only give errors when _EEPROMEX_DEBUG is set
  EEPROM.setMemPool(memBase, EEPROMSizeUno);

  // Set maximum allowed writes to maxAllowedWrites.
  // More writes will only give errors when _EEPROMEX_DEBUG is set
  EEPROM.setMaxAllowedWrites(maxAllowedWrites);

  //Inicializamos la primera dirección de la memoria
  address = EEPROM.getAddress(sizeof(byte));

  // Si el pin A5 analogico esta al aire la medida tomada
  // generará una semilla diferente cada vez que se ejecute
  // el programa.
  randomSeed(analogRead(A5));

  //Inicializo el juego
  guijarros = 21;
  contador = 0;
  anteriorValido[0] = false;
  anteriorValido[1] = false;
  mostrarMatriz();
}

/******************************************************************
                                Loop
 ******************************************************************/
void loop() {
  //El juego se repite infinitamente

  //Si todavia tengo guijarros el microcontrolador juega
  if (guijarros > 0)
  {
    guijarros = juegaPc(guijarros,Jugador1);
  } else { //Sino hay reinicio el juego y muestro la actualización del juego
    guijarros = 21;
    //Mostramos al ganador de esta partida
    Serial.println("Ha ganado el jugador 1");
    //Se muestra la matriz con las decisiones
    mostrarMatriz();
  }

  if (guijarros > 0)
  {
    guijarros = juegaPc(guijarros,Jugador2);
  } else { //Sino hay reinicio el juego y muestro la actualización del juego
    guijarros = 21;
    //Mostramos al ganador de esta partida
    Serial.println("Ha ganado el jugador 2");
    //Se muestra la matriz con las decisiones
    mostrarMatriz();
  }
}

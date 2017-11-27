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
 *                    Definicion de variables                     *
 ******************************************************************/
const int maxAllowedWrites = 80;
const int memBase          = 350;

/******************************************************************
                               Setup
 ******************************************************************/

void setup() {


  // start reading from position memBase (address 0) of the EEPROM. Set maximumSize to EEPROMSizeUno
  // Writes before membase or beyond EEPROMSizeUno will only give errors when _EEPROMEX_DEBUG is set
  EEPROM.setMemPool(memBase, EEPROMSizeUno);

  // Set maximum allowed writes to maxAllowedWrites.
  // More writes will only give errors when _EEPROMEX_DEBUG is set
  EEPROM.setMaxAllowedWrites(maxAllowedWrites);
  delay(100);

  // Inizializamos la memoria con todo 1 (posibilidad correcta) menos en los casos
  // en los que se coja mas guijarros de los que queden
  byte input  = B11011001;
  EEPROM.write(memBase,input);   // escribimos el byte
  input = 0xFF;
  for(unsigned int i = 1; i<=8 ; i++)
  {
     EEPROM.write(memBase+i,input);
  }
}

void loop() {
}

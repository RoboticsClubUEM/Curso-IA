/*  
 *
 *     ********************************************************
 *     ********************************************************
 *     ***                                                  ***
 *     ***               Artificial Inteligence             ***
 *     ***                                                  ***
 *     ******************************************************** 
 *     ******************************************************** 
 *     
 *   ****************************************************
 *   * Fecha: 23/12/2016                                *
 *   * Autor: Ruben Fernandez Pacheco                   *
 *   * Mail: r.zordke@hotmail.com                       *
 *   * Licencia: GNU General Public License v3 or later *
 *   ****************************************************
 */
 
/******************************************************************
 *                    Definition of variables                     *
 ******************************************************************/

/* Pin definition of the board to be used */

#define pinRed                            A0   
#define pinBlue                           A1
#define pinReward                          8
#define pinDiscard                         9     

/* color select */
float select;

/* threshold */
int choice;

/******************************************************************
 *                             Setup                              *
 ******************************************************************/
 
void setup() {
  /* Leds as a OUTPUT */
  pinMode(pinRed,OUTPUT);
  pinMode(pinBlue,OUTPUT);

  /* Buttons as a INPUT */
  pinMode(pinReward,INPUT);
  
  /* Inicialization of threshold */
  choice = 5;
  
  // if analog input pin 5 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(A5));
}

void loop() {
  // Turn off leds
  digitalWrite(pinRed,LOW);
  digitalWrite(pinBlue,LOW);

  // Keep leds off
  delay(500);
  
  // Rool the dice
  select = random(11);
  
  // Compare with the threshold value and turn on led
  if( select < choice) 
  {
    digitalWrite(pinRed,HIGH);
  }else{
    digitalWrite(pinBlue,HIGH);
  }

  /*  The user can reward the microcontroller pressing the button reward, 
      the selecting of that color is increased  */
  do{
    if(!digitalRead(pinReward)){
  
      if( (select < choice) && choice <= 10)
      {
        choice++;
      }
      if( !(select < choice) && choice > 0)
      {
        choice--;
      }

      // Wait for released
      do{
      }while(digitalRead(pinReward));
      
      break;
   }
  }while(digitalRead(pinDiscard));
  
  // Wait for released
  do{
  }while(!digitalRead(pinDiscard));
}

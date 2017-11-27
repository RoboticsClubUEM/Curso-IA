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
 *   * Fecha: 24/12/2016                                *
 *   * Autor: Ruben Fernandez Pacheco                   *
 *   * Mail: r.zordke@hotmail.com                       *
 *   * Licencia: GNU General Public License v3 or later *
 *   ****************************************************
 */
 
/******************************************************************
 *                    Definicion de variables                     *
 ******************************************************************/

/* Definiciones de pines usados en la placa */
#define pinWin                            A0   
#define pinDraw                           A1
#define pinLost                           A2
#define pinPaper                          8
#define pinScissors                       9
#define pinRock                           10
#define pinButton                         11

/* variables usadas para el comportamiento */
int iWin;
int myMove;
int oponentMove,oponentLastMove;
int played;

/* variables para mostrar los resultados */
int myScore,oponentScore;

/******************************************************************
 *                             Setup                              *
 ******************************************************************/
 
void setup() {
  // Configuracion de pines de salida
  digitalWrite(pinWin,OUTPUT);
  digitalWrite(pinDraw,OUTPUT);
  digitalWrite(pinLost,OUTPUT);

  // Configuracion de pines de entrada
  digitalWrite(pinPaper,INPUT);
  digitalWrite(pinScissors,INPUT);
  digitalWrite(pinRock,INPUT);
  digitalWrite(pinButton,INPUT);

  // Reinicio las variables
  myScore=0;
  oponentScore=0;
  played=0;
  
  //Inicio la conexión serie
  Serial.begin(9600);

  // Si el pin A5 analogico esta al aire la medida tomada
  // generará una semilla diferente cada vez que se ejecute
  // el programa.
  randomSeed(analogRead(A5));

  //Primer caso
  myMove = random(0,3);
  oponentLastMove = -1;

}

/******************************************************************
 *                             Loop                               *
 ******************************************************************/
 
void loop() {
  //Apago los leds
  digitalWrite(pinLost,LOW);
  digitalWrite(pinDraw,LOW);
  digitalWrite(pinWin,LOW);
    
  //Espero a que se pulse el botón
  do{
  }while(digitalRead(pinButton));
  
  //Obtengo la elección del jugador
  if(digitalRead(pinPaper) && !digitalRead(pinScissors) && !digitalRead(pinRock))
  {
    oponentMove=0;
  }else if(digitalRead(pinScissors)&& !digitalRead(pinPaper) && !digitalRead(pinRock)){
    oponentMove=1;
  }else if(digitalRead(pinRock)&& !digitalRead(pinPaper) && !digitalRead(pinScissors)){
    oponentMove=2;
  }else{
    oponentMove=-1;
  }
  
  //Si mi oponente ha elegido Calculo el resultado
  if(oponentMove!=-1)
  {
    //Si hay una posicion de diferencia gana el mayor
    //Si hay dos posiciones de diferencia gana el menor
    //Si las dos elecciones son iguales es un empate
    int auxiliar = abs(myMove-oponentMove);
    if( (auxiliar) == 1){
      if(myMove>oponentMove)
      {
        myScore+=2;
        iWin=2;
      }else{
        oponentScore+=2;
        iWin=0;
      }
    }else if (auxiliar == 2){
      if(myMove<oponentMove)
      {
        myScore+=2;
        iWin=2;
      }else{
        oponentScore+=2;
        iWin=0;
      }
    }else{
      myScore++;
      oponentScore++;
      iWin=1;
    }

    //Ilumino el led correspondiente segun lo que haya pasado
    switch (iWin)
    {
      case 0:
        digitalWrite(pinLost,HIGH);
        break;
      case 1:
        digitalWrite(pinDraw,HIGH);
        break;
      case 2:
        digitalWrite(pinWin,HIGH);
        break;
    }
  }
  
  //Espero a que se suelte el botón como medida de seguridad
  do{
  }while(!digitalRead(pinButton));

  //Calculo mi siguiente movimiento en base al anterior resultado
  if(oponentMove!=-1)
  {
    //Supongo que mi oponente no va a repetir su jugada
    //Asi que ganaré si eligo la eleccion que pierde contra su
    //elección actual
    myMove = oponentMove -1;
    if(myMove < 0)
    {
      myMove = 2;
    }
    
    //En cambio si elige siempre el mismo ganaria por lo que
    //si he perdido y el ultimo movimiento del rival es igual 
    //al anterior cambiare mi eleccion por una que gane a su 
    //actual selección
    if(oponentLastMove==oponentMove && !iWin)
    {
      myMove = oponentMove +1;
      if(myMove > 2)
      {
        myMove = 0;
      }
    }
    
    //Actualizo mi ultimo movimiento y las veces jugadas
    oponentLastMove = oponentMove;
    played++;

    //Lo devuelvo para ver el resultado en el plotter
    Serial.print(myScore);
    Serial.print(" ");
    Serial.print(oponentScore);
    Serial.print(" ");
    Serial.println(played);

    //Espero un poco antes de la siguiente ronda
    delay(500);
  }
}

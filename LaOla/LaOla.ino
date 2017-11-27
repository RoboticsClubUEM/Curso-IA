/*

 *     ********************************************************
 *     ********************************************************
 *     ***                                                  ***
 *     ***               Artificial Inteligence             ***
 *     ***                                                  ***
 *     ********************************************************
 *     ********************************************************

 *   ****************************************************
 *   * Fecha: 02/01/2017                                *
 *   * Autor: Ruben Fernandez Pacheco                   *
 *   * Mail: r.zordke@hotmail.com                       *
 *   * Licencia: GNU General Public License v3 or later *
 *   ****************************************************
*/

/******************************************************************/
/******************************************************************/


/******************************************************************
                      Definicion de variables
 ******************************************************************/

/* Definiciones de pines usados en la placa */
#define pinLDR   A0
#define pinLED   13

/* variable para el autocalibrado */
#define percentage 12
int threshold, example, randomVal;

/* variables para el aleatorismo */
#define maxRandom 11
#define capRandom 2

/* variables para el comportamiento */
#define maxTime 11
unsigned long timeElapsed, initialTime, requiredTime;

/******************************************************************
                               Setup
 ******************************************************************/

void setup() {
  Serial.begin(9600);

  //Configuro mi salida
  pinMode(pinLED, OUTPUT);

  // Si el pin A5 analogico esta al aire la medida tomada
  // generará una semilla diferente cada vez que se ejecute
  // el programa.
  randomSeed(analogRead(A5));

  // Autocalibro el threshold
  threshold = analogRead(pinLDR) * percentage / 10;


  // Apago el led
  digitalWrite(pinLED, LOW);

  // Inizicio la cuenta de la inactividad
  initialTime = millis();
  requiredTime = random(1, maxTime) * 1000;
  Serial.print("Mi tiempo requerido es: ");
  Serial.println(requiredTime);
}

/******************************************************************
                               Loop
 ******************************************************************/

void loop() {
  // Cojemos una muestra
  example = analogRead(pinLDR);

  // Generamos un valor aleatorio
  randomVal = random(1, maxRandom);

  // Comprobamos si una luz se ha detectado en el LDR
  // y si es asi encendemos el led despues de un pequeño tiempo
  if (example > threshold) {
    Serial.println("Hola");
    // Si estamos con animo participativo encendemos
    if (randomVal > capRandom)
    {
      delay(200);
      digitalWrite(pinLED, HIGH);
      delay(100);
      digitalWrite(pinLED, LOW);
      initialTime = millis();
    }
  }

  // Calculamos en nuevo valor del threshold
  threshold = example * percentage / 10;

  // Si llevo un minimo de tiempo sin hacer nada puede que
  // me apetezca iniciar la ola
  timeElapsed = millis() - initialTime;
  if ( timeElapsed > requiredTime) {
    // Encendemos el LED
    digitalWrite(pinLED, HIGH);
    delay(100);
    digitalWrite(pinLED, LOW);

    // Reiniciamos la cuenta de inactividad
    requiredTime = random(1, maxTime) * 1000;
    Serial.print("Mi tiempo requerido es: ");
    Serial.println(requiredTime);
    initialTime = millis();
  }
}

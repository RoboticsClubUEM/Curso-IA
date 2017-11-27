/*

 *     ********************************************************
 *     ********************************************************
 *     ***                                                  ***
 *     ***               Artificial Inteligence             ***
 *     ***                                                  ***
 *     ********************************************************
 *     ********************************************************

 *   ****************************************************
 *   * Fecha: 14/01/2017                                *
 *   * Autor: Ruben Fernandez Pacheco                   *
 *   * Mail: r.zordke@hotmail.com                       *
 *   * Licencia: GNU General Public License v3 or later *
 *   ****************************************************
*/

/******************************************************************/
/******************************************************************/



/******************************************************************
                             Libraries
 ******************************************************************/

#include <Servo.h>


/******************************************************************
                      Definition of variables
 ******************************************************************/

/* Pin definition of the board to be used */

#define pinLeftWheel            8
#define pinRightWheel           9
#define pinUSTri                4   /*   Ultrasound trigger       */
#define pinUSEch                5   /*   Ultrasound echo          */
#define pinHead                11   /*   Servo                    */

/* Definition of the values ​​that can take continuous rotation servo,
  that is, the wheels */
#define wheelStopValue 90
#define leftWheelFordwardValue 0
#define leftWheelBackwardsValue 180
#define rightWheelFordwardValue 180
#define rightWheelBackwardsValue 0

/* Default delay */
#define defaultDelay        10

/* A object from the Servo class is created for each servo */
Servo leftWheel;                       /*  Values from 0 to 180  */
Servo rightWheel;                      /*  Values from 0 to 180  */

/* Variables of the obstacles avoider mode */
#define US_CENTER_ANGLE                    80
#define US_LEFT_ANGLE                     110
#define US_RIGHT_ANGLE                     50
#define defaultDelayUS                     50
Servo head;                      /*  Values from 0 to 180  */

/* Variables of roulette wheel brain */
unsigned int fitnessA;
unsigned int fitnessB;
unsigned int fitnessC;
unsigned int wheel;
unsigned int select;
#define defaultFitness            10

/* Variables for behavior */
unsigned long initialTime;
#define maxTime                 2000
#define minDistance               50

/******************************************************************
                       Definition of functions
 ******************************************************************/

/* bqBAT (bq US) */
long TP_init_4_5() {

  digitalWrite(pinUSTri, LOW);
  delayMicroseconds(2);
  digitalWrite(pinUSTri, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinUSTri, LOW);
  long microseconds = pulseIn(pinUSEch , HIGH);
  return microseconds;
}

/* Calculation */
long distance_4_5() {

  long microseconds = TP_init_4_5();
  long distance;
  distance = microseconds / 29 / 2;
  return distance;
}


/* Move US */
void turnHead(int angle, int delayDuration) {

  head.write(angle);
  delay(delayDuration);
}



void stopWheels() {

  leftWheel.write(wheelStopValue);
  delay(defaultDelay);

  rightWheel.write(wheelStopValue);
  delay(defaultDelay);
}


void goForwards() {

  leftWheel.write(leftWheelFordwardValue);
  delay(defaultDelay);

  rightWheel.write(rightWheelFordwardValue);
  delay(defaultDelay);
}

void goBackwards() {

  leftWheel.write(leftWheelBackwardsValue);
  delay(defaultDelay);

  rightWheel.write(rightWheelBackwardsValue);
  delay(defaultDelay);
}


void goLeft() {

  leftWheel.write(wheelStopValue);
  delay(defaultDelay);

  rightWheel.write(rightWheelFordwardValue);
  delay(defaultDelay);
}


void goRight() {

  leftWheel.write(leftWheelFordwardValue);
  delay(defaultDelay);

  rightWheel.write(wheelStopValue);
  delay(defaultDelay);
}

void turnLeft() {
  goLeft();
  delay(900);
  stopWheels();
}


void turnRight() {
  goRight();
  delay(900);
  stopWheels();
}

/******************************************************************
                               Setup
 ******************************************************************/

void setup() {

  /* Define the appropiate pin to each object */
  leftWheel.attach(pinLeftWheel);
  rightWheel.attach(pinRightWheel);
  head.attach(pinHead);

  /* US sensors */
  pinMode(pinUSTri, OUTPUT);
  pinMode(pinUSEch, INPUT);

  // if analog input pin 5 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(A5));

  /* Initialization of roulette wheel brain */
  fitnessA = defaultFitness;
  fitnessB = fitnessA;
  fitnessC = fitnessA;

  /* The robot is stopped at the beginning */
  stopWheels();

  /* Point the head fordwards */
  turnHead(US_CENTER_ANGLE, defaultDelay);
}


/******************************************************************
                         Main program loop
 ******************************************************************/

void loop() {

  /* Select an action using the roulette wheel */
  unsigned int total = fitnessA + fitnessB + fitnessC;
  if (total == 0)
  {
    select = 0;
  } else {
    wheel = random(fitnessA + fitnessB + fitnessC);
    if (wheel <= fitnessA)
    {
      select = 1;
    } else if (wheel <= fitnessA + fitnessB) {
      select = 2;
    } else {
      select = 3;
    }
  }

  /* Perform the action */
  switch (select) {
    case 0:
      //this is a problem
      break;
    case 1:
      turnRight();
      goForwards();
      break;
    case 2:
      turnLeft();
      goForwards();
      break;
    case 3:
      goForwards();
      break;
  }

  initialTime = millis();
  long value = 0;
  
  /* If fixed time has passed we want to stop */
  do {
    value = distance_4_5();
    /* Object detected */
    if (value > 0 && value < minDistance)
    {
      /* Reduce the chance for the current action */
      if (total > 1)
      {
        switch (select) {
          case 1:
            if (fitnessA > 0)
            {
              fitnessA--;
            }
            break;
          case 2:
            if (fitnessB > 0)
            {
              fitnessB--;
            }
            break;
          case 3:
            if (fitnessC > 0)
            {
              fitnessC--;
            }
            break;
        }
      }
      break;
    }
    /* Precent overload US */
    delay(defaultDelayUS);
  } while (maxTime > (millis() - initialTime));

  /* The robot is stopped */
  stopWheels();
}

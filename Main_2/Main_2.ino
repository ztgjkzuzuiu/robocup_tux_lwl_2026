#include "CoRoSoN_Main.h"
#include "CoRoSoN_IR-Ring.h"
#include "CoRoSoN_Compass.h"
#include "CoRoSoN_Pixy.h"
#include <elapsedMillis.h>



IRRing irRing(I2C_ADD_IR);
Compass compass(I2C_ADD_Compass);
// Pixy pixy(I2C_ADD_PIXY, 1, 2);
bool IsInGame = false;
elapsedMillis buttonDeadTime = 0;

void setup() {

  pinMode(17, OUTPUT);

  delay(500);
  SetBoardLEDColor(LEFT, BLUE);
  SetBoardLEDColor(RIGHT, BLUE);

  CoRoSoN_Init();
  //pixy.Init();

  SetI2CLEDColor(I2C_MODULE_1, LEFT, RED);
  SetI2CLEDColor(I2C_MODULE_1, RIGHT, RED);

  delay(200);
  SetBoardLEDColor(LEFT, GREEN);
  SetBoardLEDColor(RIGHT, GREEN);
  delay(200);
  SetBoardLEDColor(LEFT, RED);
  SetBoardLEDColor(RIGHT, RED);
  delay(200);
  SetBoardLEDColor(LEFT, YELLOW);
  SetBoardLEDColor(RIGHT, YELLOW);
  delay(200);
  SetBoardLEDColor(LEFT, BLUE);
  SetBoardLEDColor(RIGHT, BLUE);
  delay(200);
  SetBoardLEDColor(LEFT, CYAN);
  SetBoardLEDColor(RIGHT, CYAN);
  delay(200);
  SetBoardLEDColor(LEFT, MAGENTA);
  SetBoardLEDColor(RIGHT, MAGENTA);
  delay(200);
  SetBoardLEDColor(LEFT, WHITE);
  SetBoardLEDColor(RIGHT, WHITE);
  delay(200);
  SetBoardLEDColor(LEFT, OFF);
  SetBoardLEDColor(RIGHT, OFF);
  delay(500);
}

void kick() {
  digitalWrite(17, HIGH);
  delay(30);
  digitalWrite(17, LOW);
}


bool lightbarrierTriggered() {
  return (analogRead(ANALOG_IN_4) > 1000);
}

void drive(int speed, int rotation) {
  int MaxSpeed = ABS(speed) + ABS(rotation);
  if (MaxSpeed > 100) {
    speed = (speed * 100) / MaxSpeed;
    rotation = (rotation * 100) / MaxSpeed;
  }
  int spd1 = rotation - speed;
  int spd2 = rotation + speed;
  DIRECTION dir1 = (spd1 >= 0 ? FORWARD : BACKWARD);
  DIRECTION dir2 = (spd2 >= 0 ? FORWARD : BACKWARD);
  SetMotor(MOTOR_1, dir1, spd1);
  SetMotor(MOTOR_2, dir2, spd2);
}

void standby() {
  //
  // Status LED Lightbarrier
  //

  /*  if (lightbarrierTriggered()) {
    SetI2CLEDColor(I2C_MODULE_1, LEFT, GREEN);
  } else {
    SetI2CLEDColor(I2C_MODULE_1, LEFT, OFF);
  }
*/

  //
  // Status LED ball
  //
  if (irRing.BallSeen()) {
    if (ABS(irRing.BallDirection()) < 3) {
      SetBoardLEDColor(LEFT, GREEN);
    } else if (ABS(irRing.BallDirection()) < 15 && ABS(irRing.BallDirection()) >= 3) {
      SetBoardLEDColor(LEFT, CYAN);
    } else {
      SetBoardLEDColor(LEFT, BLUE);
    }
  } else {
    SetBoardLEDColor(LEFT, RED);
  }

  if (compass.Angle() < 90) {
    SetBoardLEDColor(RIGHT, BLUE);
  } else if (compass.Angle() >= 90 && compass.Angle() < 180) {
    SetBoardLEDColor(RIGHT, CYAN);
  } else if (compass.Angle() >= 180 && compass.Angle() < 270) {
    SetBoardLEDColor(RIGHT, MAGENTA);
  } else {
    SetBoardLEDColor(RIGHT, YELLOW);
  }

  //
  // Button left => drive test
  //
  if (ButtonIsPressed(I2C_MODULE_1, LEFT)) {
    drive(50, 0);
    SetMotor(MOTOR_3, FORWARD, 50);
  } else {
    drive(0, 0);
    SetMotor(MOTOR_3, FORWARD, 0);
  }
  //
  // Button right => enter game
  //
  SetI2CLEDColor(I2C_MODULE_1, RIGHT, GREEN);
  if (ButtonIsPressed(I2C_MODULE_1, RIGHT)) {
    if (buttonDeadTime > 500) {
      IsInGame = true;
      buttonDeadTime = 0;
      //
      // Shut off all LEDs
      //
      SetBoardLEDColor(RIGHT, OFF);
      SetI2CLEDColor(I2C_MODULE_1, LEFT, OFF);
      SetI2CLEDColor(I2C_MODULE_1, RIGHT, OFF);
    }
  }
}
//***theo begin***
void rechts()
{
SetMotor(MOTOR_1, BACKWARD, 25);
SetMotor(MOTOR_2, BACKWARD, 25);
SetMotor(MOTOR_3, FORWARD, 49);
}

void links()
{
SetMotor(MOTOR_1, FORWARD, 25); 
SetMotor(MOTOR_2, FORWARD, 29);
SetMotor(MOTOR_3, BACKWARD, 57);
}

void vorwarz()
{
SetMotor(MOTOR_1, BACKWARD, 50);
SetMotor(MOTOR_2, FORWARD, 50);
}
//***theo end***


void game() {

int lichtschranke = analogRead(4);//mit lightbarrier triggered austauschen
int schwellenwert_licht = 1000;



/*SetMotor(MOTOR_1, BACKWARD, 25);
SetMotor(MOTOR_2, BACKWARD, 25);*/

  // Game strategy
  //
  
  //  TODO: Insert your game strategy..

 // while(lichtschranke > schwellenwert_licht)  // 1.Abfrage ist ball in licht schranke

  
  //Motor Kabel mit schwarzer seite anschließen
  
    /*if(ABS(irRing.BallDirection())  <= 5 && irRing.BallDirection() < 32){                                                                     //Wenn zwischen 5 und -5 geradeaus fahren
       SetMotor(MOTOR_1, BACKWARD, 10);
        SetMotor(MOTOR_2, FORWARD, 10);
    } else if(irRing.BallDirection() > -5 && irRing.BallDirection() < 32 && irRing.BallDirection() > -30){                                   //Wenn größer als 30 und kleiner als -5 diago nach hinten links fahren
    SetMotor(MOTOR_1, FORWARD, 10);
    SetMotor(MOTOR_3, BACKWARD, 10);
    } else if(irRing.BallDirection()> 5 && irRing.BallDirection()<= 30){                                                                     //Wenn kleiner gleich 30 und größer als 5 diago nach hinten links fahren
         SetMotor(MOTOR_2, BACKWARD, 10);
        SetMotor(MOTOR_3, FORWARD, 10);
    } else if(irRing.BallDirection()> 30 || irRing.BallDirection() <= -30 ){                                                                                                  //                                                                                               
          SetMotor(MOTOR_2, BACKWARD, 10);
          SetMotor(MOTOR_3, FORWARD, 10);
    } else if(irRing.BallDirection()== -32){                                                                                                 //Wenn Ball nich sieht fahr nach hinten
            SetMotor(MOTOR_1, FORWARD, 10);
            SetMotor(MOTOR_2, BACKWARD, 10);
    }*/


 /*Serial.println(analogRead(4));
 delay(200);*/
}

void loop() {

  if (IsInGame) {
    game();
    //
    // Button right => enter game
    //
    if (ButtonIsPressed(I2C_MODULE_1, LEFT) || ButtonIsPressed(I2C_MODULE_1, RIGHT)) {
      if (buttonDeadTime > 500) {
        IsInGame = false;
        buttonDeadTime = 0;
        //SetMotor(MOTOR_4, FORWARD, 0);
      }
    }
  } else {
    standby();
  }



  //
  // Debug prints
  //
  /* DEBUG_PRINT(irRing.BallSeen());
  DEBUG_PRINT(irRing.BallDirection());
  DEBUG_PRINT(irRing.BallDistance());
  DEBUG_PRINT(pixy.GoalSeen());
  DEBUG_PRINT(pixy.GoalDirection());
  DEBUG_PRINT(pixy.OwnGoalSeen());
  DEBUG_PRINT(pixy.OwnGoalDirection());*/
  //
  // Update
  //

  compass.Update();
  irRing.Update();
  //pixy.Update();
  delay(5);
}
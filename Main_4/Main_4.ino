#include "CoRoSoN_Main.h"
#include "CoRoSoN_IR-Ring.h"
#include "CoRoSoN_Compass.h"
#include "CoRoSoN_Pixy.h"
#include <elapsedMillis.h>
#include "DFRobot_URM09.h"


IRRing irRing(I2C_ADD_IR);
Compass compass(I2C_ADD_Compass);
// Pixy pixy(I2C_ADD_PIXY, 1, 2);
bool IsInGame = false;
elapsedMillis buttonDeadTime = 0;
elapsedMillis antiStuck;
elapsedMillis messComp;

int rohComp3;
int rohComp;
int offset;
int offset2;
int diroffset;
int diroffset2;
bool usSide;
bool usFront;
int mC_1;
int mC_2;
int mC_3;
int mC_4;
int mC_5;
int mC_6;

/*DFRobot_URM09 URM09_l;
DFRobot_URM09 URM09_r;
DFRobot_URM09 URM09_v;
DFRobot_URM09 URM09_h;*/

float kP = 0.5;
float kI = 0;  //0.005;
float kD = 0.7;

float lastDiff = 0;
float Iterm = 0;

void setup() {
  /*URM09_l.begin(0x12);
  URM09_r.begin(0x13);
  URM09_v.begin(0x14);
  URM09_h.begin(0x15);*/

  pinMode(17, OUTPUT);
  //***theo begin***
  /**
   * The module is configured in automatic mode or passive
   *  MEASURE_MODE_AUTOMATIC       automatic mode
   *  MEASURE_MODE_PASSIVE         passive mode
   * The measurement distance is set to 500,300,150 
   *  MEASURE_RANG_500             Ranging from 500 
   *  MEASURE_RANG_300             Ranging from 300 
   *  MEASURE_RANG_150             Ranging from 150 
  */
  /*URM09_l.setModeRange(MEASURE_MODE_AUTOMATIC, MEASURE_RANG_300);
  URM09_r.setModeRange(MEASURE_MODE_AUTOMATIC, MEASURE_RANG_300);
  URM09_v.setModeRange(MEASURE_MODE_AUTOMATIC, MEASURE_RANG_300);
  URM09_h.setModeRange(MEASURE_MODE_AUTOMATIC, MEASURE_RANG_300);*/
  //delay(100);
  //***theo end***

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

  compass.Update();
  int rohComp2 = ((int)compass.Angle());
  offset = (180 - rohComp2 + 360) % 360;
}


float berechnePID(float abweichungKompassrichtung) {
  Iterm += abweichungKompassrichtung;  // collects permanent deviation
  float Dterm = abweichungKompassrichtung - lastDiff;
  lastDiff = abweichungKompassrichtung;
  return kP * abweichungKompassrichtung + kI * Iterm + kD * Dterm;
}

int powerlimiter(int power, int minVal) {
  if (power < 0) {
    return 0;
  }
  return power;
}


void kick() {
  digitalWrite(17, HIGH);
  delay(30);
  digitalWrite(17, LOW);
}


bool lightbarrierTriggered() {
  return (analogRead(ANALOG_IN_4) > 1400);
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
void rechts2(int power) {
  rohComp = ((int)compass.Angle());
  int aktComp = (rohComp + offset) % 360;
  int korrektur = (int)berechnePID(diroffset2);
  SetMotor(MOTOR_1, BACKWARD, powerlimiter(0.58 * (power - korrektur), 0));  //geschwindigkeit verringern wenn ballsensor programm 0.51
  SetMotor(MOTOR_2, BACKWARD, powerlimiter(0.58 * (power - korrektur), 0));
  SetMotor(MOTOR_3, FORWARD, powerlimiter(power + korrektur, 0));
}

void links2(int power) {
  rohComp = ((int)compass.Angle());
  int aktComp = (rohComp + offset) % 360;
  int korrektur = (int)berechnePID(diroffset2);
  SetMotor(MOTOR_1, FORWARD, powerlimiter(0.58 * (power + korrektur), 0));
  SetMotor(MOTOR_2, FORWARD, powerlimiter(0.58 * (power + korrektur), 0));
  SetMotor(MOTOR_3, BACKWARD, powerlimiter(power - korrektur, 0));
}

void vorwaerts2(int power) {
  rohComp = ((int)compass.Angle());
  int aktComp = (rohComp - 180 + offset) % 360;
  int korrektur = (int)berechnePID(diroffset);
  SetMotor(MOTOR_1, BACKWARD, powerlimiter(power - korrektur, 0));
  SetMotor(MOTOR_2, FORWARD, powerlimiter(power + korrektur, 0));
  SetMotor(MOTOR_3, FORWARD, 0);
}

void rueckwaerts2(int power) {
  rohComp = ((int)compass.Angle());
  int aktComp = (rohComp + offset) % 360;
  int korrektur = (int)berechnePID(diroffset2);
  SetMotor(MOTOR_1, FORWARD, powerlimiter(power + korrektur, 0));
  SetMotor(MOTOR_2, BACKWARD, powerlimiter(power - korrektur, 0));
  SetMotor(MOTOR_3, FORWARD, 0);
}

void rechts(int power) {
  rohComp = ((int)compass.Angle());
  int aktComp = (rohComp + offset) % 360;
  int korrektur = (int)berechnePID(diroffset);
  SetMotor(MOTOR_1, BACKWARD, powerlimiter(0.58 * (power - korrektur), 0));  //geschwindigkeit verringern wenn ballsensor programm 0.51
  SetMotor(MOTOR_2, BACKWARD, powerlimiter(0.58 * (power - korrektur), 0));
  SetMotor(MOTOR_3, FORWARD, powerlimiter(power + korrektur, 0));
  /* SetMotor(MOTOR_1, BACKWARD, 25);
  SetMotor(MOTOR_2, BACKWARD, 25);
  SetMotor(MOTOR_3, FORWARD, 49);*/
  /*SetMotor(MOTOR_1, BACKWARD, 13);
  SetMotor(MOTOR_2, BACKWARD, 13);
  SetMotor(MOTOR_3, FORWARD, 25);*/
}

void links(int power) {
  rohComp = ((int)compass.Angle());
  int aktComp = (rohComp + offset) % 360;
  int korrektur = (int)berechnePID(diroffset);
  SetMotor(MOTOR_1, FORWARD, powerlimiter(0.58 * (power + korrektur), 0));
  SetMotor(MOTOR_2, FORWARD, powerlimiter(0.58 * (power + korrektur), 0));
  SetMotor(MOTOR_3, BACKWARD, powerlimiter(power - korrektur, 0));
  /*SetMotor(MOTOR_1, FORWARD, 25);
  SetMotor(MOTOR_2, FORWARD, 29);
  SetMotor(MOTOR_3, BACKWARD, 57);*/
  /*SetMotor(MOTOR_1, FORWARD, 13);
  SetMotor(MOTOR_2, FORWARD, 15);
  SetMotor(MOTOR_3, BACKWARD, 26);*/
}

void vorwaerts(int power) {
  rohComp = ((int)compass.Angle());
  int aktComp = (rohComp + offset) % 360;
  int korrektur = (int)berechnePID(diroffset);
  SetMotor(MOTOR_1, BACKWARD, powerlimiter(power - korrektur, 0));
  SetMotor(MOTOR_2, FORWARD, powerlimiter(power + korrektur, 0));
  SetMotor(MOTOR_3, FORWARD, 0);
}

void rueckwaerts(int power) {
  rohComp = ((int)compass.Angle());
  int aktComp = (rohComp + offset) % 360;
  int korrektur = (int)berechnePID(diroffset);
  SetMotor(MOTOR_1, FORWARD, powerlimiter(power + korrektur, 0));
  SetMotor(MOTOR_2, BACKWARD, powerlimiter(power - korrektur, 0));
  SetMotor(MOTOR_3, FORWARD, 0);
}

void turnLeft(int speed) {
  if (speed > 25) {
    speed = 25;
  } else if (speed < 10) {
    speed = 10;
  }
  SetMotor(MOTOR_1, FORWARD, speed);
  SetMotor(MOTOR_2, FORWARD, speed);
  SetMotor(MOTOR_3, FORWARD, speed);
}


void turnRight(int speed) {
  if (speed > 25) {
    speed = 25;
  } else if (speed < 10) {
    speed = 10;
  }
  SetMotor(MOTOR_1, BACKWARD, speed);
  SetMotor(MOTOR_2, BACKWARD, speed);
  SetMotor(MOTOR_3, BACKWARD, speed);
}

void compaass() {

  rohComp = ((int)compass.Angle());
  int aktComp = (rohComp + offset) % 360;

  if (aktComp > 10 && aktComp <= 180) {
    turnLeft((aktComp - 180) / 6);
  } else if (aktComp <= 350 && aktComp >= 180) {
    turnRight(ABS(aktComp - 180) / 6);
  } else if (aktComp <= 195 && aktComp >= 165) {
    SetMotor(MOTOR_1, FORWARD, 30);
    SetMotor(MOTOR_2, BACKWARD, 30);
    SetMotor(MOTOR_3, FORWARD, 0);
  }
}
//***theo end***


void game() {
  //0x12-0x15

  rohComp = ((int)compass.Angle());
  int aktComp = (rohComp + offset) % 360;
  diroffset = aktComp - 180;
/*
Serial.println(rohComp);

  if (messComp == 500) {
  rohComp = mC_1;

} else if (messComp == 1000) {
  rohComp = mC_2;
  if (abs(mC_2 - mC_1) > 1) {
    messComp = 0;
  }

} else if (messComp == 1500) {
  rohComp = mC_3;
  if (abs(mC_3 - mC_2) > 1) {
    messComp = 0;
  }

} else if (messComp == 2000) {
  rohComp = mC_4;
  if (abs(mC_4 - mC_3) > 1) {
    messComp = 0;
  }

} else if (messComp == 2500) {
  rohComp = mC_5;
  if (abs(mC_5 - mC_4) > 1) {
    messComp = 0;
  }

} else if (messComp == 3000) {
  rohComp = mC_6;
  if (abs(mC_6 - mC_5) > 1) {
    messComp = 0;
  } else {
    rechts(100);
  }
}*/




  /*Serial.println(String("offset ")+ diroffset);
Serial.println(String("aktComp ")+ aktComp);
Serial.println(String("rohcomp ")+ rohComp);
Serial.println();
delay(1000);*/
  //vorwaerz();
  //compaass();
  /*SetMotor(MOTOR_1, BACKWARD, 25);
SetMotor(MOTOR_2, BACKWARD, 25);*/
  //if(delayfahr < 4000){
  //}else if(delayfahr > 4000 && delayfahr < 8000){
  //links(30);
  //}else{
  // delayfahr = 0;
  //}
  // Game strategy
  //
  //SetMotor(MOTOR_4, FORWARD, 100);dribbler

  //  TODO: Insert your game strategy..

  // 1.Abfrage ist ball in licht schranke true = ja    false = nein

  //Serial.println(irRing.BallDirection());
  //Motor Kabel mit schwarzer seite anschließen


  if (ABS(irRing.BallDirection()) <= 4) {  //Wenn zwischen 5 und -5 geradeaus fahren
    vorwaerts(40);
    Serial.println("vor");
  } else if (irRing.BallDirection() < -4 && irRing.BallDirection() > -17) {  //Wenn größer als 30 und kleiner als -5 diago nach links fahren
    links(50);
    Serial.println("links");
  } else if (irRing.BallDirection() > 4 && irRing.BallDirection() <= 17) {  //Wenn kleiner gleich 30 und größer als nach rechts fahren
    rechts(50);
    Serial.println("rechts");
  } else if (irRing.BallDirection() == -32 || irRing.BallDirection() <= -17 && irRing.BallDirection() >= -28) {
    rueckwaerts(50);
    Serial.println("ruck");
  } else if (irRing.BallDirection() > 17 && irRing.BallDirection() <= 27) {  //neue ruckwarts line wegen unubersichtlich
    rueckwaerts(50);
    Serial.println("ruck");
  } else if (irRing.BallDirection() < -28 && irRing.BallDirection() > -32 || irRing.BallDirection() > 27) {
    rechts(35);
    Serial.println("rechts");
  }


  //SetMotor(MOTOR_4, FORWARD, 100);
  /*  int16_t dist_l = URM09_l.getDistance();
  int16_t dist_r = URM09_r.getDistance();
  int16_t dist_v = URM09_v.getDistance();
  int16_t dist_h = URM09_h.getDistance();

if(dist_l + dist_r > 160 && (aktComp > 350 || aktComp < 10)){
 usSide = true;
}else{
  usSide = false;
}

if(dist_v + dist_h > 210 && (aktComp > 350 || aktComp < 10)){
  usFront = true;
}else{
  usFront = false;
}
/*
if(usSide == true){

}

SetMotor(MOTOR_4, FORWARD, 100);
/*
Serial.println(dist_l + dist_r);
Serial.println(aktComp);
Serial.print(usFront);
Serial.println(usSide);
delay(1000);*/
  /*
  Serial.println(dist_l, DEC);
  Serial.println(dist_r, DEC);
 // Serial.println(dist_v, DEC);
 // Serial.println(dist_h, DEC);
  Serial.println("cm");
  delay(1000);*/

  //Serial.println(analogRead(4));
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
  delay(2);
  irRing.Update();
  //pixy.Update();
  delay(5);
}
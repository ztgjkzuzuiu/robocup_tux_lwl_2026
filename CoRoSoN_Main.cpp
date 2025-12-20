#include "esp32-hal-ledc.h"
// -------------------------------------------------------------------
/*********************************************************************
* 
* !    LICENSE
*         GNU GPL 3.0
*
*********************************************************************/
// -------------------------------------------------------------------
/*********************************************************************
* 
* *     CoRoSoN
* *       Co|mponents for 
* *       Ro|bocupJunior
* *       So|ccer
* *        N|ewcomers
*
* *    Project:     https://github.com/CoRoSoN-Kit/CoRoSoN-Kit
* *    Competition: https://www.robocup.org/
*
* *    Created by BohleBots:
* *       https://www.bohlebots.de
* *       info@bohlebots.de
* *
* *    Some Function were modified by tux 
* 
*********************************************************************/
// -------------------------------------------------------------------
// File:    CoRoSoN.cpp
// Purpose: Implementations for CoRoSoN.h
// -------------------------------------------------------------------
/*********************************************************************
* 
*  Dependencies
*
*********************************************************************/
#include "CoRoSoN_Main.h"
/*********************************************************************
* 
*  Implementations
*
*********************************************************************/
static byte _LEDModule1Left  = 0;
static byte _LEDModule1Right = 0;
static byte _LEDModule2Left  = 0;
static byte _LEDModule2Right = 0;

Adafruit_NeoPixel onBoardLed = Adafruit_NeoPixel(onBoardLedNum, onBoardLedPin, NEO_GRB + NEO_KHZ800);

ERRORS CoRoSoN_Init() {
  ERRORS r;
  int    i;
  
  r = OKAY;
  i = 0;
  //
  // Serial Monitor
  //
  Serial.begin(115200);
  //
  // on Board Led
  //
                     // onBoardLed.begin();
  //
  // I2C
  //
  r |= I2C_Init(I2C_SDA, I2C_SCL);
  if(r) {
    r |= (CONNECT_FAILED | ERROR_IGNORED);
    DEBUG_ERRORS(r);
    DEBUG_BLOCK("I2C Init failed", 1000);
  }
  //
  // Button and LED modules
  //
  r |= I2C_TestConnection(I2C_MODULE_1);
  if(r) {
    r |= (CONNECT_FAILED | ERROR_IGNORED);
    DEBUG_ERRORS(r);
  }
  r |= I2C_TestConnection(I2C_MODULE_2);
  if(r) {
    r |= (CONNECT_FAILED | ERROR_IGNORED);
    DEBUG_ERRORS(r);
  }
  //
  // Pins
  //
  // Motors
  pinMode(MOTOR_DIS, OUTPUT); digitalWrite(MOTOR_DIS, LOW);
  pinMode(MOTOR_1_DIR, OUTPUT);
  pinMode(MOTOR_2_DIR, OUTPUT);
  pinMode(MOTOR_3_DIR, OUTPUT);
  pinMode(MOTOR_4_DIR, OUTPUT);
  ledcAttach(MOTOR_1_PWM, 1000, 8);
  ledcAttach(MOTOR_2_PWM, 1000, 8);
  ledcAttach(MOTOR_3_PWM, 1000, 8);
  ledcAttach(MOTOR_4_PWM, 1000, 8);
  ledcWrite(MOTOR_1_PWM, 0);
  ledcWrite(MOTOR_2_PWM, 0);
  ledcWrite(MOTOR_3_PWM, 0);
  ledcWrite(MOTOR_4_PWM, 0);
  // Analog
  pinMode(ANALOG_IN_1, INPUT);
  pinMode(ANALOG_IN_2, INPUT);
  pinMode(ANALOG_IN_3, INPUT);
  pinMode(ANALOG_IN_4, INPUT);
  // LEDs onBoard
  
  // Digital
  pinMode(DIGITAL_OUT_1, OUTPUT); digitalWrite(DIGITAL_OUT_1, LOW);
  pinMode(DIGITAL_OUT_2, OUTPUT); digitalWrite(DIGITAL_OUT_2, LOW);
  return r;
}

ERRORS SetBoardLEDColor(SIDE Side, COLOR Color) {
  switch(Side) {
    case LEFT: 
      switch(Color) {
        case OFF:
          onBoardLed.setPixelColor(0, onBoardLed.Color(0, 0, 0));
        break;
        case RED:
          onBoardLed.setPixelColor(0, onBoardLed.Color(255, 0, 0));
        break;
        case GREEN:
          onBoardLed.setPixelColor(0, onBoardLed.Color(0, 255, 0));
        break;
        case YELLOW:
          onBoardLed.setPixelColor(0, onBoardLed.Color(255, 255, 0));
        break;
        case BLUE:
          onBoardLed.setPixelColor(0, onBoardLed.Color(0, 0, 255));
        break;
        case CYAN:
          onBoardLed.setPixelColor(0, onBoardLed.Color(0, 255, 255));
        break;
        case MAGENTA:
          onBoardLed.setPixelColor(0, onBoardLed.Color(255, 0, 255));
        break;
        case WHITE:
          onBoardLed.setPixelColor(0, onBoardLed.Color(255, 255, 255));
        break;
      }
      break;
    case RIGHT: 
      switch(Color) {
        case OFF:
          onBoardLed.setPixelColor(1, onBoardLed.Color(0, 0, 0));
        break;
        case RED:
          onBoardLed.setPixelColor(1, onBoardLed.Color(255, 0, 0));
        break;
        case GREEN:
          onBoardLed.setPixelColor(1, onBoardLed.Color(0, 255, 0));
        break;
        case YELLOW:
          onBoardLed.setPixelColor(1, onBoardLed.Color(255, 255, 0));
        break;
        case BLUE:
          onBoardLed.setPixelColor(1, onBoardLed.Color(0, 0, 255));
        break;
        case CYAN:
          onBoardLed.setPixelColor(1, onBoardLed.Color(0, 255, 255));
        break;
        case MAGENTA:
          onBoardLed.setPixelColor(1, onBoardLed.Color(255, 0, 255));
        break;
        case WHITE:
          onBoardLed.setPixelColor(1, onBoardLed.Color(255, 255, 255));
        break;
      }
      break;
  }
  onBoardLed.show();
  return OKAY;
}

ERRORS SetI2CLEDColor(I2C_ADD_BTNLED_MODULE AddModule, SIDE Side, COLOR Color) {
  ERRORS r;
  byte   bColor;
  byte   aMessage[1];

  r = OKAY;
  bColor = (byte)Color;
  ZEROMEM(aMessage);
  switch(AddModule) {
    case I2C_MODULE_1: 
      switch(Side) {
        case LEFT: 
          _LEDModule1Left = bColor * 2; 
          break;
        case RIGHT: bColor *= 16; 
          if(bColor > 63) {
            bColor += 64;
           } 
           _LEDModule1Right = bColor; 
           break;
      }
      aMessage[0] = 255 - _LEDModule1Left - _LEDModule1Right; 
      break;
    case I2C_MODULE_2: 
      switch(Side) {
        case LEFT: 
          _LEDModule2Left  = bColor * 2; 
          break;
        case RIGHT: bColor *= 16; 
          if(bColor > 63) {
            bColor += 64;
           } 
           _LEDModule2Right = bColor; 
           break;
      }
      aMessage[0] = 255 - _LEDModule2Left - _LEDModule2Right; 
      break;
  }
  r |= I2C_Write(AddModule, aMessage, ARRAY_LENGTH(aMessage));
  if(r) {
    r |= CONNECT_FAILED;
    DEBUG_ERRORS(r);
  }
  return r;
}

bool ButtonIsPressed(I2C_ADD_BTNLED_MODULE AddModule, SIDE Side) {
  bool   IsPressed;
  ERRORS r;
  byte   aAnswer[1];

  IsPressed = false;
  r = OKAY;
  ZEROMEM(aAnswer);
  r |= I2C_ReadBlocking(AddModule, aAnswer, 1);
  if(r) {
    r |= ERROR_BREAK_OUT;
    DEBUG_ERRORS(r);
    return false;
  }
  switch(Side) {
    case LEFT : IsPressed = ((~aAnswer[0]) & (1 << 0)); break; // left  btn presed =>  1-bit is 0
    case RIGHT: IsPressed = ((~aAnswer[0]) & (1 << 6)); break; // right btn presed => 64-bit is 0
  }
  return IsPressed;
}

ERRORS SetMotor(MOTOR Motor, DIRECTION Direction, unsigned int SpeedPercentage) {
  ERRORS r;

  r = OKAY;
  if(SpeedPercentage > 100) {
    r |=  INVALID_PARAMETER | ERROR_HANDLED;
    DEBUG_ERRORS(r);
    DEBUG_PRINT(SpeedPercentage);
    SpeedPercentage = 100;
  }
  switch(Motor) {
    case MOTOR_1: digitalWrite(MOTOR_1_DIR, Direction); ledcWrite(MOTOR_1_PWM, (SpeedPercentage * 255) / 100); break;
    case MOTOR_2: digitalWrite(MOTOR_2_DIR, Direction); ledcWrite(MOTOR_2_PWM, (SpeedPercentage * 255) / 100); break;
    case MOTOR_3: digitalWrite(MOTOR_3_DIR, Direction); ledcWrite(MOTOR_3_PWM, (SpeedPercentage * 255) / 100); break;
    case MOTOR_4: digitalWrite(MOTOR_4_DIR, Direction); ledcWrite(MOTOR_4_PWM, (SpeedPercentage * 255) / 100); break;
  }
  return r;
}
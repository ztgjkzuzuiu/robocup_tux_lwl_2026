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
* *    Competition: https://www.robocuIRRingp.org/
*
* *    Created by tux
* 
*********************************************************************/
// -------------------------------------------------------------------
// File:    CoRoSoN_Compass.cpp
// Purpose: Implementations for CoRoSoN_Compass.h
// -------------------------------------------------------------------
/*********************************************************************
* 
*  Dependencies
*
*********************************************************************/
#include "CoRoSoN_Compass.h"
/*********************************************************************
* 
*  Implementations
*
*********************************************************************/

Compass::Compass(unsigned short Address) {
  ZEROMEM(this->mPriv);
  this->mPriv.Address       = Address;
  this->mPriv.Angle = 0.0;
}

ERRORS Compass::Update() {
  ERRORS r;
  byte   aAnswer[2];
  unsigned int angle16;
  byte aMessageBytes[1];

  r = OKAY;
  ZEROMEM(aAnswer);

  aMessageBytes[0] = ANGLE;   
  
  I2C_Write(this->mPriv.Address, aMessageBytes, 1); 

  r = I2C_ReadBlocking(this->mPriv.Address, aAnswer, ARRAY_LENGTH(aAnswer));
  if(r) {
    r |= INVALID_ANSWER | ERROR_BREAK_OUT;
    DEBUG_ERRORS(r);
    return r;
  }

  angle16 = aAnswer[0];                 // Calculate 16 bit angle
  angle16 <<= 8;
  angle16 += aAnswer[1];

  this->mPriv.Angle = angle16/10.0;

  return r;
}


float Compass::Angle() {
  return this->mPriv.Angle;
}


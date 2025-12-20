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
* *    Created by tux
* 
*********************************************************************/
// -------------------------------------------------------------------
// File:    CoRoSoN_Kompass.h
// Purpose: Main header for the compass provided by CoRoSoN IR-Ring addition
// -------------------------------------------------------------------
#ifndef COROSON_COMPASS_H
#define COROSON_COMPASS_H
/*********************************************************************
* 
*  Dependencies
*
*********************************************************************/
#include "CoRoSoN_Util.h"
#include "CoRoSoN_I2C.h"

#define ANGLE  2           // Register to read 8bit angle from

/*********************************************************************
* 
*  Types
*
*********************************************************************/
class Compass {
public:
  /************************************************************
  *
  * ? Compass()
  *   
  * * Description:
  *     Constructor
  *
  ************************************************************/
  Compass(unsigned short Address);

  /************************************************************
  *
  * ? Update()
  *   
  * * Description:
  *     Reads the new data
  *
  ************************************************************/
  ERRORS Update(void);

 
  /************************************************************
  *
  * ? Angle()
  *   
  * * Angle:
  *     Returns the angle of the compass as follows:
  *       0 to 359.9 degrees
  *
  ************************************************************/
  float Angle(void);

  private:
  struct {
    unsigned short Address;
             float Angle;
  } mPriv;
};
#endif // COROSON_Compass_H
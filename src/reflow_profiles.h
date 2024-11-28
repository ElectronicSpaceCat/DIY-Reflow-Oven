/*******************************************************************************
    MIT License

    Copyright (c)  2021-present  Andrew Green

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

 *******************************************************************************/

#ifndef _REFLOW_PROFILES_H
#define _REFLOW_PROFILES_H

#include "Arduino.h"

namespace REFLOW {

#define NUM_PROFILES 5

  typedef enum
  {
    ZONE_PREHEAT = 0,
    ZONE_SOAK,
    ZONE_REFLOW,
    ZONE_COOL,
    NUM_ZONES
  } zone_t;

  // ***** TYPES *****
  typedef struct{
     double kp;
     double ki;
     double kd;
  }pid_coefficient_t;

  typedef struct {
    unsigned long temp;
    unsigned char time;
  } profile_setpoint_t;

  typedef struct {
    profile_setpoint_t set;
    pid_coefficient_t pid;
  } pidData_t;

  typedef struct{
    String name;
    pidData_t data[NUM_ZONES];
  }profileData_t;

  extern const profileData_t profileData[NUM_PROFILES];

  byte getProfileID(void);
  void setProfileID(byte id);
  const profileData_t* getCurrentProfile(void);
}

#endif // _REFLOW_PROFILES_H

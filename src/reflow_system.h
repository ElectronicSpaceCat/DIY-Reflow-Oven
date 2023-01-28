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

/**
 * The file contains the pinout for the Arduino Mega 2560 Rev3
 *
 * Adjust them as desired but make sure you know what you are doing.
 */

#ifndef _REFLOW_SYSTEM_IO_H
#define _REFLOW_SYSTEM_IO_H

#include "Arduino.h"

namespace REFLOW::SYSTEM{

  enum PIN{
    HEARTBEAT_LED       = LED_BUILTIN,
    SSR_FAN             = 9,
    SSR_HEATER_TOP      = 10,
    SSR_HEATER_BOTTOM	= 11,
    PUSH_BTN_PROFILE	= 12,
  };

  void init(void);
  void poll(void);

  void ledHeartBeatToggle(void);
  void ssrFanON(void);
  void ssrFanOFF(void);
  void ssrHeaterTopON(void);
  void ssrHeaterTopOFF(void);
  void ssrHeaterBottomON(void);
  void ssrHeaterBottomOFF(void);
}

#endif // _REFLOW_SYSTEM_IO_H

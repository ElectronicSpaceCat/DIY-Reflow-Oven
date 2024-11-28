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

#ifndef _OLED_GFX_H
#define _OLED_GFX_H

#include "reflow_profiles.h"
#include "reflow_state_machine.h"

namespace REFLOW::OLED::GFX {

  void begin(void);
  void display(void);
  void clearDisplay(void);

  namespace DRAW {

    void splash(void);
    void state(SM::state_t state);
    void defaultData(const profileData_t* profile);
    void profileData(const profileData_t* profile);
    void temperature(double temp);
    void temperatureSetpoint(double temp);
    void setXstartPoint(double xAxis);
    void plotTemperature(double temp);
    void reflowTime(unsigned long time);
    void zoneTime(unsigned long time);
    void zoneLine(void);
    void xAxis(void);
    void yAxis(void);

  }

}

#endif // _OLED_GFX_H

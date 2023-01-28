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
   The DIY Reflow app was designed to run on the Arduino Mega 2560 Rev3 (which was in hand at the time)
   but could likely be modified to run on other Arduino hardware.
  
   The purpose of the application is to provide control of a DIY reflow oven using a PID control algorithm. It uses a single button
   as a menu/select interface. A long press enters a profile selection menu. In this menu, a quick presses will cycle
   through 5x predefined profiles. A long press in this menu will return to the main screen.
 
    Required libraries
   ==============================
   - Arduino  PID:
   https://github.com/br3ttb/Arduino-PID-Library

   - Adafruit SSD1306:
   https://github.com/adafruit/Adafruit_SSD1306

   - Adafruit GFX:
   https://github.com/adafruit/Adafruit-GFX-Library

   - Adafruit BusyIO:
   https://github.com/adafruit/Adafruit_BusIO

   - Adafruit_MCP9600:
   https://github.com/adafruit/Adafruit_MCP9600

   - thomasfredericks bounce2:
   https://github.com/thomasfredericks/Bounce2

   - arduino-libraries LiquidCrystal:
   https://github.com/arduino-libraries/LiquidCrystal
   
 */

#include "reflow_common.h"
#include "reflow_oled.h"
#include "reflow_debug.h"
#include "reflow_pid.h"
#include "reflow_led.h"
#include "reflow_buttons.h"
#include "reflow_system.h"
#include "reflow_thermocouple.h"
#include "reflow_state_machine.h"

void setup(void) {
  REFLOW::SYSTEM::init();
  REFLOW::LED::init();
  REFLOW::THERMOCOUPLE::init();
  REFLOW::BUTTONS::init();
  REFLOW::PID::init();
  REFLOW::DEBUG::init();
  REFLOW::OLED::init();
  REFLOW::SM::init();
}

void loop(void) {
  REFLOW::SYSTEM::poll();
  REFLOW::LED::poll();
  REFLOW::THERMOCOUPLE::poll();
  REFLOW::BUTTONS::poll();
  REFLOW::PID::poll();
  REFLOW::DEBUG::poll();
  REFLOW::OLED::poll();
  REFLOW::SM::poll();
}

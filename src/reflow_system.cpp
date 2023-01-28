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

#include "reflow_system.h"

namespace REFLOW::SYSTEM{

void init(void){
    // SSR top heater pin initialization
    pinMode(PIN::SSR_HEATER_TOP, OUTPUT);
    digitalWrite(PIN::SSR_HEATER_TOP, LOW);

    // SSR bottom heater pin initialization
    pinMode(PIN::SSR_HEATER_BOTTOM, OUTPUT);
    digitalWrite(PIN::SSR_HEATER_BOTTOM, LOW);

    // SSR fan pin initialization
    pinMode(PIN::SSR_FAN, OUTPUT);
    digitalWrite(PIN::SSR_FAN, LOW);

    // LED pins initialization and turn on upon start-up (active high)
    pinMode(PIN::HEARTBEAT_LED, OUTPUT);
    digitalWrite(PIN::HEARTBEAT_LED, LOW);

    // Enable and setup watchdog so we reset if hang-up occurs
    watchdogSetup();
    watchdogEnable(2000);
  }

  void poll(void){
    // Kick the dog
    watchdogReset();
  }

  void ledHeartBeatToggle(void){
    digitalWrite(PIN::HEARTBEAT_LED, !(digitalRead(PIN::HEARTBEAT_LED)));
  }

  void ssrFanON(void){
    digitalWrite(PIN::SSR_FAN, HIGH);
  }

  void ssrFanOFF(void){
    digitalWrite(PIN::SSR_FAN, LOW);
  }

  void ssrHeaterTopON(void){
    digitalWrite(PIN::SSR_HEATER_TOP, HIGH);
  }

  void ssrHeaterTopOFF(void){
    digitalWrite(PIN::SSR_HEATER_TOP, LOW);
  }

  void ssrHeaterBottomON(void){
    digitalWrite(PIN::SSR_HEATER_BOTTOM, HIGH);
  }

  void ssrHeaterBottomOFF(void){
    digitalWrite(PIN::SSR_HEATER_BOTTOM, LOW);
  }
}

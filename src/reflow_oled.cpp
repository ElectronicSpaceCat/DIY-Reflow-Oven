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

#include "reflow_oled.h"
#include "reflow_oled_gfx.h"
#include "reflow_profiles.h"
#include "reflow_thermocouple.h"
#include "reflow_pid.h"

/*
 * NOTE:
 * Updating the oled faster than 1Hz seems
 * to cause program delays. Try to keep calls
 * to GFX::display() at 1Hz or less.
 */
namespace {
  const int INTERVAL_TIME = 1000;
  const int MAX_DISPLAY_TIME = 999;

  unsigned long timerOLED = 0;
  bool signalUpdate = false;

  void resetScreen(void);
}

namespace REFLOW::OLED {

  void init(void) {
    // Start-up splash
    GFX::begin();
    GFX::clearDisplay();
    GFX::DRAW::splash();
    GFX::display();
    delay(2500);
    GFX::clearDisplay();

    // Initialize debug ouput
    timerOLED = millis();
  }

  void poll(void) {
    static REFLOW::SM::state_t locStatePrev = SM::STATE_ERROR;
    static byte locProfilePrev = 0;
    static unsigned long locTimerReflowPrev = -1;

    // Update reflow data if in progress
    if(SM::getStatus() == SM::STATUS_ON) {
      unsigned long locTimerReflow = SM::get_timerReflow();

      if(locTimerReflowPrev != locTimerReflow) {
        locTimerReflowPrev = locTimerReflow;
        // Update oled and saturate at upper time limit
        if(SM::get_timerReflow() <= MAX_DISPLAY_TIME) {
          GFX::DRAW::reflowTime(locTimerReflow);
          GFX::DRAW::zoneTime(SM::get_timerZone());
        }

        // Plot temperature point every 3 seconds
        if((locTimerReflow % 3) == 0) {
          GFX::DRAW::plotTemperature(THERMOCOUPLE::getTemperature());
        }
        GFX::DRAW::temperature(THERMOCOUPLE::getTemperature());
        GFX::DRAW::temperatureSetpoint(PID::getPID_setPoint());

        signalUpdate = true;
      }
    }
    // While not in a reflow, up date the display at periodic intervals
    else if((millis() - timerOLED) >= INTERVAL_TIME) {
      // Update next time to update oled
      timerOLED += INTERVAL_TIME;
      // Update temperature at periodic rate
      GFX::DRAW::temperature(THERMOCOUPLE::getTemperature());
      signalUpdate = true;
    }
    else {
      // ...
    }

    SM::state_t locState = SM::getState();

    // Update state driven data
    if(locStatePrev != locState) {
      locStatePrev = locState;
      GFX::DRAW::state(locState);
      // Reset screen
      if(locState == SM::STATE_RESET) {
        resetScreen();
      }
      // Draw section line indicating start of next zone
      else if(locState == SM::STATE_SOAK || locState == SM::STATE_REFLOW || locState == SM::STATE_COOL) {
        GFX::DRAW::zoneLine();
      }
      // Reset stored profile state to trigger a graphic update when entering the profile screen
      else if(locState == SM::STATE_PROFILE){
        locProfilePrev = 0xFF;
      }
      else {
        //...
      }
      signalUpdate = true;
    }

    // Update profile if changed
    byte locProfile = getProfileID();
    if(locProfilePrev != locProfile) {
      locProfilePrev = locProfile;
      if(locState == SM::STATE_PROFILE) {
        GFX::DRAW::profileData(getCurrentProfile());
      }
      else {
        GFX::DRAW::defaultData(getCurrentProfile());
      }
      signalUpdate = true;
    }

    // Refresh screen when signaled
    if(signalUpdate == true) {
      signalUpdate = false;
      GFX::display();
    }
  }
}

namespace {
  void resetScreen(void) {
    using namespace REFLOW;
    using namespace REFLOW::OLED;
    GFX::clearDisplay();
    GFX::DRAW::defaultData(getCurrentProfile());
    GFX::DRAW::state(SM::getState());
    GFX::DRAW::temperature(THERMOCOUPLE::getTemperature());
    // Reset the plot starting point
    GFX::DRAW::setXstartPoint(0);
    // Update screen
    GFX::display();
  }
}

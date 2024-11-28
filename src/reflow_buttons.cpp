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

#include "reflow_buttons.h"
#include "reflow_profiles.h"
#include "reflow_system.h"
#include "Bounce2.h"

// ***** VARIABLES *****
namespace {
  Bounce2::Button button_1 = Bounce2::Button();
  REFLOW::BUTTONS::btnPress_t _buttonPress = REFLOW::BUTTONS::PRESS_NA;
}

namespace REFLOW::BUTTONS {

  void init(void) {
    // Push button to toggle between main and profile selection screens
    button_1.attach(SYSTEM::PIN::PUSH_BTN_PROFILE, INPUT_PULLUP);
    button_1.interval(50);
    button_1.setPressedState(LOW);
  }

  void poll(void) {
    static bool buttonFlag = false;
    // Update state
    button_1.update();
    // Check if long press
    if(button_1.isPressed() && (button_1.duration() > 1200)) {
      if(buttonFlag == false) {
	buttonFlag = true;
	_buttonPress = PRESS_LONG;
      }
    }
    // If flag was set by long press, then
    // don't process the following button release
    else if(button_1.rose()) {
      if(buttonFlag == false) {
	_buttonPress = PRESS_SHORT;
      }
    }
    else {
      buttonFlag = false;
    }
  }

  btnPress_t getButtonPress(void){
    btnPress_t locButtonState = _buttonPress;
    _buttonPress = PRESS_NA;
    return locButtonState;
  }

}

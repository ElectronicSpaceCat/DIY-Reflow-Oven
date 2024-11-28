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

// ***** INCLUDES *****
#include "reflow_state_machine.h"
#include "reflow_thermocouple.h"
#include "reflow_buttons.h"
#include "reflow_system.h"
#include "reflow_pid.h"
#include "reflow_oled.h"
#include "reflow_debug.h"
#include "reflow_profiles.h"

namespace {
// ***** DEFINES *****
  const int TEMPERATURE_ROOM = 50;
  const int ONE_SECOND = 1000;
  const int INTERVAL_TIME = ONE_SECOND;
  const int PID_TIME_PERIOD_SIZE = 2000;
  const int PID_SAMPLE_TIME = 1000;
// ***** INTERFACE VARIABLES *****
  REFLOW::SM::state_t _state = REFLOW::SM::STATE_RESET;
  REFLOW::SM::status_t _status = REFLOW::SM::STATUS_OFF;
  REFLOW::zone_t _zone = REFLOW::ZONE_PREHEAT;
// ***** ACCESSOR VARIABLES *****
  bool _startSignal = false;
  unsigned long _timerReflow = 0;
  unsigned long _timerZone = 0;
// ***** VARIABLES *****
  unsigned long timerSecond = 0;
  unsigned long timerSM = 0;
// ***** FUNCTION PROTOTYPES *****
  void runStateMachine(void);
  void handleButtonSignal(void);
}

namespace REFLOW::SM {

  const char *reflowStateStrings[] =
    { "Profile", "Reset", "Debug", "Abort", "Ready", "Start", "Preheat", "Soak", "Reflow", "Cool", "Complete!", "Hot!", "Error" };

  void init(void) {
    // Initialize timer
    timerSecond = timerSM = millis();
  }

  void poll(void) {
    // Handle external signals that
    // alter the state machine
    handleButtonSignal();

    // Update reflow timers
    if(_status == SM::STATUS_ON) {
      if((millis() - timerSecond) >= ONE_SECOND) {
        timerSecond += ONE_SECOND;
        _timerReflow++;
        _timerZone++;
      }
    }

    // Running the state machine around 1 second gives
    // time for the oled logic to display the states
    // as they transition
    if((millis() - timerSM) >= INTERVAL_TIME) {
      timerSM += INTERVAL_TIME;
      // Run state machine
      runStateMachine();
    }
  }
}

namespace {
  void runStateMachine(void) {
    using namespace REFLOW;
    using namespace REFLOW::SM;

    // Get temperature
    double locTemp = THERMOCOUPLE::getTemperature();
    // Get profile index
    const pidData_t *locPID = getCurrentProfile()->data;

    // Run state machine
    switch(_state) {
      case STATE_RESET:
        // Reset timers
        _timerReflow = 0;
        _timerZone = 0;
        // Turn the PID off
        PID::setPID_enable(false);
        // Reset status to off
        _status = STATUS_OFF;
        // Set zone
        _zone = ZONE_PREHEAT;
        // Set state
        _state = STATE_IDLE;
        // Turn off fan
        SYSTEM::ssrFanOFF();

        break;

      case STATE_IDLE:
        // Is temperature above recommended safe level?
        if(locTemp >= TEMPERATURE_ROOM) {
          // No - Set state to hot
          _state = STATE_HOT;
        }
        else if(DEBUG::getDebugMode() == true) {
          _state = STATE_DEBUG;
        }
        else if(_startSignal == true) {
          _startSignal = false;
          // Yes - Start reflow
          _state = STATE_START;
        }
        else {
          // ...
        }

        break;

      case STATE_START:
        // Set PID range from 0 to period size (ms)
        PID::setPID_limits(0, PID_TIME_PERIOD_SIZE);
        // Set PID sample time
        PID::setPID_sampleTime(PID_SAMPLE_TIME);
        // Turn the PID on
        PID::setPID_enable(true);
        // Reset zone timer
        _timerZone = 0;
        // Set zone
        _zone = ZONE_PREHEAT;
        // Set temperature setpoint
        PID::setPID_setPoint(locPID[_zone].set.temp);
        // Set pid terms
        PID::setPID_terms(locPID[_zone].pid.kp, locPID[_zone].pid.ki, locPID[_zone].pid.kd);
        // Set status
        _status = STATUS_ON;
        // Reset timer
        timerSecond = millis();
        // Set state
        _state = STATE_PREHEAT;
		
        break;

      case STATE_PREHEAT:
        // If minimum soak temperature is achieve
        if(locTemp >= locPID[_zone].set.temp) {
          // Reset zone timer
          _timerZone = 0;
          // Set zone
          _zone = ZONE_SOAK;
          // Set temperature setpoint
          PID::setPID_setPoint(locPID[_zone].set.temp);
          // Set pid terms
          PID::setPID_terms(locPID[_zone].pid.kp, locPID[_zone].pid.ki, locPID[_zone].pid.kd);
          // Set State
          _state = STATE_SOAK;
        }

        break;

      case STATE_SOAK:
        if(locTemp >= locPID[_zone].set.temp) {
          // Reset zone timer
          _timerZone = 0;
          // Set zone
          _zone = ZONE_REFLOW;
          // Set temperature setpoint
          PID::setPID_setPoint(locPID[_zone].set.temp);
          // Set pid terms
          PID::setPID_terms(locPID[_zone].pid.kp, locPID[_zone].pid.ki, locPID[_zone].pid.kd);
          // Set State
          _state = STATE_REFLOW;
        }

        break;

      case STATE_REFLOW:
        if(locTemp >= locPID[_zone].set.temp) {
          // Reset zone timer
          _timerZone = 0;
          // Set zone
          _zone = ZONE_COOL;
          // Set temperature setpoint
          PID::setPID_setPoint(locPID[_zone].set.temp);
          // Set pid terms
          PID::setPID_terms(locPID[_zone].pid.kp, locPID[_zone].pid.ki, locPID[_zone].pid.kd);
          // Set State
          _state = STATE_COOL;
        }

        break;

      case STATE_COOL:
        // If minimum cool temperature is achieve
        if(locTemp < locPID[_zone].set.temp) {
          // Turn off reflow process
          _status = STATUS_OFF;
          // Proceed to reflow Completion state
          _state = STATE_COMPLETE;
        }
        else {
          // Turn on fan
          SYSTEM::ssrFanON();
        }

        break;

      case STATE_COMPLETE:
        // Temperature dropped below room temperature?
        if(locTemp < TEMPERATURE_ROOM) {
          // Turn off fan..remain here until user acknowledges
          SYSTEM::ssrFanOFF();
        }
        else {
          // No - Turn on fan
          SYSTEM::ssrFanON();
        }

        break;

      case STATE_HOT:
        // Temperature dropped below room temperature?
        if(locTemp < TEMPERATURE_ROOM) {
          // Yes - Return to idle
          _state = STATE_IDLE;
          // Turn off fan
          SYSTEM::ssrFanOFF();
        }
        else {
          // No - Turn on fan
          SYSTEM::ssrFanON();
        }

        break;

      case STATE_PROFILE:
        // Stay here until user resets

        break;

      case STATE_DEBUG:
        if(DEBUG::getDebugMode() == false) {
          _state = STATE_RESET;
        }

        break;

      case STATE_ABORT:
      case STATE_ERROR:
      default:
        _state = STATE_RESET;

        break;
    }
  }

  /* Handles the reset signal */
  void handleButtonSignal(void) {
    using namespace REFLOW;
    using namespace REFLOW::SM;

    static BUTTONS::btnPress_t buttonState;

    // Do not process button signals while in debug mode
    if(DEBUG::getDebugMode() == true) {
      return;
    }

    buttonState = BUTTONS::getButtonPress();

    switch(buttonState) {
      case BUTTONS::PRESS_LONG:

        // Switch to profile mode if in idle
        if(_state == STATE_IDLE) {
          _state = STATE_PROFILE;
        }
        else if(_status == STATUS_ON) {
          _state = STATE_ABORT;
          // Reset status to off
          _status = STATUS_OFF;
        }
        // Reset state machine for any other case
        else {
          _state = STATE_RESET;
        }
        // Force reset the state machine interval timer
        timerSM = millis();

        break;

      case BUTTONS::PRESS_SHORT:

        if(_state == SM::STATE_PROFILE) {
          // Cycle to next profile
          byte locProfileId = getProfileID();
          if(locProfileId < NUM_PROFILES - 1) {
            locProfileId++;
          }
          else {
            locProfileId = 0;
          }
          setProfileID(locProfileId);
        }
        // Abort reflow if one is active
        else if(_status == STATUS_ON) {
          _state = STATE_ABORT;
          // Reset status to off
          _status = STATUS_OFF;
        }
        // Send start signal if system is ready
        else if(_state == SM::STATE_IDLE) {
          _startSignal = true;
        }
        // Reset state machine for any other case
        else {
          _state = STATE_RESET;
        }
        // Force reset the state machine interval timer
        timerSM = millis();

        break;

      default:
        // ...
        break;

    }
  }
}

// ***** ACCESSORS *****
namespace REFLOW::SM {
  state_t getState(void) {
    return _state;
  }
  status_t getStatus(void) {
    return _status;
  }
  zone_t getZone(void) {
    return _zone;
  }
  unsigned long get_timerReflow(void) {
    return _timerReflow;
  }
  unsigned long get_timerZone(void) {
    return _timerZone;
  }
  // For serial command to override physical button press
  void setStartSignal(void) {
    if(_state == SM::STATE_IDLE) {
      _startSignal = true;
    }
  }
  // For serial command to override physical button press
  void setStopSignal(void) {
    if(_status == SM::STATUS_ON) {
      _state = STATE_ABORT;
      // Reset status to off
      _status = STATUS_OFF;
      timerSM = millis();
    }
  }
}

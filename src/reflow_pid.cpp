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

#include "reflow_pid.h"
#include "reflow_profiles.h"
#include "PID_v1.h"
#include "reflow_state_machine.h"
#include "reflow_thermocouple.h"
#include "reflow_system.h"
#include "reflow_debug.h"

/*
 * -PID period at 2000ms
 * -PID sample rate at 1000ms
 *
 * PID will control the time to turn on the heater element.
 * If the time is > (millis() - futureTime),
 * then turn on the element.
 *
 * Element(ON/OFF)
 *
 *         .----------.--- PID "on-times" within period
 *         v          v   
 *    |<------->|   |<->'        |
 *    |         '   |   '        |
 *    |         '   |   '        |
 *    |         '   |<------------------ Period (2000ms)
 *    |         '   |   '        |    |
 *    |         '   |   '        |<---'
 *  1 |_________'   |___'        |
 *    |         |   |   |        |
 *  0 |_________|___|__ |________.____
 *    0      1      2      3     4
 *             Time(s)
 *
 */

namespace {
double _setpoint = 0;
double _input = 0;
double _output = 0;
double _period = 0;

unsigned long timerPeriod = 0;

PID _pid(&_input, &_output, &_setpoint, 0, 0, 0, DIRECT);
}

namespace REFLOW::PID {

void init(void) {
    // Initialize timer
    timerPeriod = millis();
}

void poll(void) {
    // Only run PID logic when reflow in progress
    if (SM::getStatus() == SM::STATUS_ON) {
        // Time to take new input sample?
        if (_pid.Compute()) {
            // Yes - Read new sample
            _input = THERMOCOUPLE::getTemperature();
        }
        // Get current time
        static unsigned long locTimePID;
        locTimePID = (millis() - timerPeriod);
        // Did current period expire?
        if (locTimePID >= _period) {
            // Yes - Update for next period
            timerPeriod += _period;
        }
        // Is output greater than period timer
        if (_output > locTimePID) {
            // Yes - Turn on ssr
            SYSTEM::ssrHeaterTopON();
            SYSTEM::ssrHeaterBottomON();
        }
        else {
            // No - Turn off ssr
            SYSTEM::ssrHeaterTopOFF();
            SYSTEM::ssrHeaterBottomOFF();
        }
    }
    // Only force off when debug mode not active
    else if (DEBUG::getDebugMode() == false) {
        // Ensure heaters are off
        SYSTEM::ssrHeaterTopOFF();
        SYSTEM::ssrHeaterBottomOFF();
    }
    else {
        // ...
    }
}

void setPID_setPoint(double setpoint) {
    _setpoint = setpoint;
}
double getPID_setPoint(void) {
    return _setpoint;
}
double getPID_input(void) {
    return _input;
}
double getPID_output(void) {
    return _output;
}
void setPID_limits(double lower, double upper) {
    _period = upper;
    _pid.SetOutputLimits(lower, upper);
}
void setPID_sampleTime(int sampleTime) {
    _pid.SetSampleTime(sampleTime);
}
void setPID_terms(double kp, double ki, double kd) {
    _pid.SetTunings(kp, ki, kd);
}
void setPID_enable(bool enable) {
    if (enable) {
        _pid.SetMode(AUTOMATIC);
    }
    else {
        _pid.SetMode(MANUAL);
    }
}

}

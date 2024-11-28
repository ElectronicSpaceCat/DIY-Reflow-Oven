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

#include "reflow_thermocouple.h"
#include "reflow_profiles.h"
#include "Adafruit_MCP9600.h"

namespace {
  const int INTERVAL_TIME = 500;

  Adafruit_MCP9600 thermocouple;
  unsigned long timerReadData;
  double _temperature = 0;
  double _temperatureAmbient = 0;
}

namespace REFLOW::THERMOCOUPLE {
  void init(void) {
    // Initialize thermocouple interface
    thermocouple.begin();
    thermocouple.setADCresolution(MCP9600_ADCRESOLUTION_18);
    thermocouple.setThermocoupleType(MCP9600_TYPE_K);
    thermocouple.setFilterCoefficient(3);
    thermocouple.setAlertTemperature(1, 260);
    thermocouple.configureAlert(1, true, true);  // alert 1 enabled, rising temp
    thermocouple.enable(true);

    // Initialize timer
    timerReadData = millis();
  }

  void poll(void) {
    // Time to read thermocouple?
    if((millis() - timerReadData) >= INTERVAL_TIME) {
      // Update next time to read thermocouple
      timerReadData += INTERVAL_TIME;
      // Read current temperature
      _temperature = thermocouple.readThermocouple();
      // Read current ambient temperature
      _temperatureAmbient = thermocouple.readAmbient();
    }
  }

  double getTemperature(void) {
    return _temperature;
  }

  double getTemperatureAmbient(void) {
    return _temperatureAmbient;
  }
}

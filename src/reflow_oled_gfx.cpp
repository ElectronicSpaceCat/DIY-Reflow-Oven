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

#include "reflow_oled_gfx.h"
#include "Adafruit_SSD1306.h"

namespace {
  const int OLED_I2C_ADDR       = 0x3C;                 // OLED default i2c address
  const int SCREEN_WIDTH        = 128;	                // OLED display width in pixels
  const int SCREEN_HEIGHT       = 64;	                // OLED display height in pixels
  const int X_AXIS              = 18;	                // X-axis starting position
  const int X_AXIS_START        = (X_AXIS + 1);         // Data plot start
  const int X_AXIS_END          = (SCREEN_WIDTH - 1);   // Data plot end
  const int TEXT_SIZE_1_WIDTH   = 6;	                // All other sizes are multiples of this
  const int TEXT_SIZE_1_HEIGHT  = 8;                    // All other sizes are multiples of this

// oled driver
  Adafruit_SSD1306 _oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
// Status flag to reset starting x-axis point
  int16_t xStartPoint = X_AXIS_START;
}

namespace REFLOW::OLED::GFX {

  void begin(void) {
    _oled.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR);
  }

  void display(void) {
    _oled.display();
  }

  void clearDisplay(void) {
    _oled.clearDisplay();
  }

  namespace DRAW {

    void splash(void) {
      _oled.setTextSize(1);
      _oled.setTextColor(WHITE);
      _oled.setCursor(30, 0);
      _oled.println(F("Reflow Oven"));
      _oled.setCursor(27, 9);
      _oled.println(F("B&D TO1785SG")); // NOTE: Change as desired, this was the name/model of the toasted used
    }

    void state(SM::state_t state) {
      _oled.writeFillRect(25, 0, 54, 8, BLACK);
      _oled.setTextSize(1);
      _oled.setCursor(25, 0);
      _oled.print(SM::reflowStateStrings[state]);
    }

    void profileData(const profileData_t *profile) {
      // Clear profile type
      _oled.writeFillRect(0, 0, 24, 8, BLACK);
      // Clear graph
      _oled.writeFillRect(0, 18, 128, 46, BLACK);
      // Draw profile name
      _oled.setTextSize(1);
      _oled.setCursor(0, 0);
      _oled.print(F(profile->name.c_str()));
      _oled.print(F(":"));
      // Fill out the profile data table
      _oled.setCursor(10, 18);
      _oled.print(F("Temp"));
      _oled.setCursor(0, 26);
      _oled.print(F("P:"));
      _oled.print(profile->data[REFLOW::ZONE_PREHEAT].set.temp, 10);
      _oled.setCursor(0, 34);
      _oled.print(F("S:"));
      _oled.print(profile->data[REFLOW::ZONE_SOAK].set.temp, 10);
      _oled.setCursor(0, 42);
      _oled.print(F("R:"));
      _oled.print(profile->data[REFLOW::ZONE_REFLOW].set.temp, 10);
      _oled.setCursor(0, 50);
      _oled.print(F("C:"));
      _oled.print(profile->data[REFLOW::ZONE_COOL].set.temp, 10);
    }

    void defaultData(const profileData_t *profile) {
      // Clear profile type
      _oled.writeFillRect(0, 0, 24, 8, BLACK);
      // Clear graph
      _oled.writeFillRect(0, 18, 128, 46, BLACK);
      // Draw profile name
      _oled.setTextSize(1);
      _oled.setCursor(0, 0);
      _oled.print(F(profile->name.c_str()));
      _oled.print(F(":"));
      // Update temperature markers
      // 50deg is a good start point for the temperature graph
      double tmin = 50;
      // Calculate max and mid values and add a buffer of 10deg to the max
      double tmax = profile->data[REFLOW::ZONE_REFLOW].set.temp + 10;
      double tmid = tmin + ((tmax - tmin) / 2);
      // Draw temperatures on y-axis
      _oled.setCursor(0, X_AXIS);
      _oled.print(tmax, 0);
      _oled.setCursor(0, 36);
      _oled.print(tmid, 0);
      _oled.setCursor(0, 54);
      _oled.print(F(" "));
      _oled.print(tmin, 0);
      // Draw x-axis
      xAxis();
      // Draw y-axis
      yAxis();
    }

    void temperature(double temp) {
      _oled.setTextSize(1);
      // Clear the spot where temperature is printed
      _oled.writeFillRect(80, 0, 48, 8, BLACK);
      // Right align temperature reading
      if(temp < 10)
        _oled.setCursor(90, 0);
      else if(temp < 100)
        _oled.setCursor(86, 0);
      else
        _oled.setCursor(80, 0);
      // Display current temperature
      _oled.print(temp);
      _oled.print((char) 247);
      _oled.print(F("C"));
    }

    void temperatureSetpoint(double temp) {
      _oled.setTextSize(1);
      // Clear the spot where temperature is printed
      _oled.writeFillRect(68, 9, 60, 8, BLACK);
      // Display prefix
      _oled.setCursor(68, 9);
      _oled.print(F("S:"));
      // Right align temperature reading
      if(temp < 10)
        _oled.setCursor(90, 9);
      else if(temp < 100)
        _oled.setCursor(86, 9);
      else
        _oled.setCursor(80, 9);
      // Display current temperature
      _oled.print(temp);
      _oled.print((char) 247);
      _oled.print(F("C"));
    }

    void setXstartPoint(double xAxis) {
      // Clear plot area
      _oled.writeFillRect(X_AXIS_START, 18, 108, 45, BLACK);
      // Set the new start position
      if(xAxis < X_AXIS_START) {
        xStartPoint = X_AXIS_START;
      }
      else if(xAxis > X_AXIS_END) {
        xStartPoint = X_AXIS_END;
      }
      else {
        // ...
      }
    }

    void plotTemperature(double temp) {
      // Are we under the x-axis boundary?
      if(xStartPoint < 127) {
        // Yes - Plot the temperature
        int16_t tempMapped = (int16_t) map(temp, 0, 230, 63, 19);
        _oled.drawPixel(xStartPoint++, tempMapped, WHITE);
      }
      // Reset the plot point to the start
      // if we're still plotting data
      else {
        setXstartPoint(0);
      }
    }

    void reflowTime(unsigned long time) {
      _oled.writeFillRect(0, 9, 30, 8, BLACK);
      _oled.setCursor(0, 9);
      _oled.print(F("R:"));
      _oled.print(time);
    }

    void zoneTime(unsigned long time) {
      _oled.writeFillRect(33, 9, 30, 8, BLACK);
      _oled.setCursor(33, 9);
      _oled.print(F("Z:"));
      _oled.print(time);
    }

    void zoneLine(void) {
      // Draw zone line divider
      // Offset by 2 pixels to give dotted line
      for(int16_t y = X_AXIS; y < 63; y += 2) {
        _oled.drawPixel(xStartPoint, y, WHITE);
      }
    }

    void xAxis(void) {
      _oled.drawLine(X_AXIS, 63, 127, 63, WHITE);
    }

    void yAxis(void) {
      _oled.drawLine(X_AXIS, 18, 18, 63, WHITE);
    }

  }
}

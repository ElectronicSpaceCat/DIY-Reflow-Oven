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
 * This code is what runs the command line debugging interface. A program like Putty
 * can be used to work with it. Simply type "help" for command options.
 *
 * Connection parameters:
 * comm:        whatever port is in use
 * baud rate:   115200
 * putty:       force echo on to see what is being typed
 */

#include "reflow_debug.h"
#include "reflow_common.h"
#include "reflow_state_machine.h"
#include "reflow_thermocouple.h"
#include "reflow_system.h"
#include "reflow_pid.h"

namespace {
  const int INTERVAL_TIME = 1000;

  char cmd[50];
  unsigned long timerDebug;
  bool _debugMode = false;
  bool printHeader = true;

  void printDebug(void);
  void readCmds(void);
  void processCmd(void);
  void printHelp(void);
}

namespace REFLOW::DEBUG {

  void init(void) {
    timerDebug = millis();
    Serial.begin(115200);
    Serial.println("DIY-Reflow-Oven");
    printHelp();
  }

  void poll(void) {
    // Time to read print debug data?
    if((millis() - timerDebug) >= INTERVAL_TIME) {
      // Update timer
      timerDebug += INTERVAL_TIME;
      printDebug();
    }
    // Process any user commands
    readCmds();
  }

  bool getDebugMode(void) {
    return _debugMode;
  }

}

namespace {
  void printDebug(void) {
    using namespace REFLOW;

    // If reflow process is on going
    if(SM::getStatus() == SM::STATUS_ON) {
      if(printHeader == true) {
        printHeader = false;
        Serial.println(F("ReflowTime,Setpoint,Input,Output"));
      }
      // Send temperature and time stamp to serial
      Serial.print(SM::get_timerReflow());
      Serial.print(F(","));
      Serial.print(PID::getPID_setPoint());
      Serial.print(F(","));
      Serial.print(PID::getPID_input());
      Serial.print(F(","));
      Serial.println(PID::getPID_output());
    }
    else {
      printHeader = true;
    }
  }

  void readCmds(void) {
    using namespace REFLOW;
    static unsigned int idx = 0;
    if(Serial.available()) {
      char locChr = Serial.read();
      // Line feed or carriage return?
      if(locChr == 0x0A || locChr == 0x0D) {
        // Yes - Add null terminator then process command
        cmd[idx] = '\0';
        processCmd();
        // Reset index
        idx = 0;
      }
      else if(idx < sizeof(cmd)) {
        cmd[idx++] = locChr;
      }
      else {
        idx = 0;
      }
    }
  }

  void processCmd(void) {
    using namespace REFLOW;

    /// PRINT COMMANDS
    if(!strcmp(cmd, "help")) {
      printHelp();
    }
    /// TOGGLE RUN MODE
    else if(!strcmp(cmd, "debug")) {
      if(SM::getState() == SM::STATE_IDLE) {
        _debugMode = true;
        Serial.println(">> debug mode enabled");
        printHelp();
      }
      else if(SM::getState() == SM::STATE_DEBUG) {
        _debugMode = false;
        Serial.println(">> debug mode disabled");
      }
      else {
        Serial.println(">> must be in idle mode to enable");
      }
    }
    else if(!strcmp(cmd, "next profile")) {
      if(SM::getState() == SM::STATE_IDLE) {
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
      Serial.print(">> profile: ");
      Serial.println(getCurrentProfile()->name.c_str());
    }
    else if(SM::getState() != SM::STATE_DEBUG && !strcmp(cmd, "start")) {
      if(SM::getState() == SM::STATE_IDLE) {
        SM::setStartSignal();
        Serial.println(">> reflow started");
      }
      else {
        Serial.println(">> must be in idle mode to start");
      }
    }
    else if(!strcmp(cmd, "stop")) {
      if(SM::getStatus() == SM::STATUS_ON) {
        SM::setStopSignal();
        Serial.println(">> reflow stopped");
      }
    }
    /// FAN ON
    else if(SM::getState() == SM::STATE_DEBUG && !strcmp(cmd, "fan on")) {
      SYSTEM::ssrFanON();
      Serial.println(">> fan on");
    }
    /// FAN OFF
    else if(SM::getState() == SM::STATE_DEBUG && !strcmp(cmd, "fan off")) {
      SYSTEM::ssrFanOFF();
      Serial.println(">> fan off");
    }
    /// HEATER 1 ON
    else if(SM::getState() == SM::STATE_DEBUG && !strcmp(cmd, "h1 on")) {
      SYSTEM::ssrHeaterTopON();
      Serial.println(">> heater 1 on");
    }
    /// HEATER 1 OFF
    else if(SM::getState() == SM::STATE_DEBUG && !strcmp(cmd, "h1 off")) {
      SYSTEM::ssrHeaterTopOFF();
      Serial.println(">> heater 1 off");
    }
    /// HEATER 2 ON
    else if(SM::getState() == SM::STATE_DEBUG && !strcmp(cmd, "h2 on")) {
      SYSTEM::ssrHeaterBottomON();
      Serial.println(">> heater 2 on");
    }
    /// HEATER 2 OFF
    else if(SM::getState() == SM::STATE_DEBUG && !strcmp(cmd, "h2 off")) {
      SYSTEM::ssrHeaterBottomOFF();
      Serial.println(">> heater 2 off");
    }
    /// GET TEMPERATURES
    else if(!strcmp(cmd, "get temps")) {
      Serial.print("TC: ");
      Serial.print(THERMOCOUPLE::getTemperature());
      Serial.print(", Amb: ");
      Serial.println(THERMOCOUPLE::getTemperatureAmbient());
    }
    else {
      Serial.println(">> unknown command");
    }
  }

  void printHelp(void) {
    Serial.println("");
    if(_debugMode) {
      Serial.println("Debug Commands:");
    }
    else {
      Serial.println("Commands:");
    }
    Serial.println(" debug");
    Serial.println(" next profile");
    Serial.println(" get temps");
    if(_debugMode) {
      Serial.println(" fan on/off");
      Serial.println(" h1 on/off");
      Serial.println(" h2 on/off");
    }
    else {
      Serial.println(" start/stop");
    }
    Serial.println("");
  }
}

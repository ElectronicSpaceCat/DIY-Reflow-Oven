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
 * NOTE: This file is meant to replace reflow_debug.cpp for testing purposes.
 *
 *       It allows fine tuning of the PID controller.
 */


#include "reflow_debug.h"
#include "reflow_common.h"
#include "reflow_state_machine.h"
#include "reflow_thermocouple.h"
#include "reflow_system.h"
#include "reflow_pid.h"

#define INTERVAL_TIME 1000

#define DEBUG_OUTPUT
//#define DEBUG_TEMP

#ifdef DEBUG_OUTPUT
#define DEBUG_PRINT_CSV_DATA
#undef DEBUG_TEMP
#endif

typedef enum {
  DATA_PID_SETPOINT = 0, DATA_PID_KP, DATA_PID_KI, DATA_PID_KD, NUM_DATA_ENTRIES
} data_entry_t;

static bool _debugMode = false;
static double _setpoint, _kp, _ki, _kd = 0;

static unsigned long timerDebug;
static bool printHeader = true;
static char cmd[50];
static data_entry_t dataEntry = NUM_DATA_ENTRIES;

static void printDebug(void);
static void readCmds(void);
static void processCmd(void);
static void printHelp(void);
static void processDataEntry(void);

namespace REFLOW::DEBUG {

  void init(void) {
    timerDebug = millis();
    Serial.begin(115200);
  }

  void poll(void) {
    // Time to read print debug data?
    if((millis() - timerDebug) >= INTERVAL_TIME) {
      // Update next time to read thermocouple
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

void printDebug(void) {
  using namespace REFLOW;
#ifdef DEBUG_TEMP
    Serial.print("HJ: "); Serial.print(thermocouple.readThermocouple()); Serial.print(" ");
    Serial.print("CJ: "); Serial.print(thermocouple.readAmbient()); Serial.print(" ");
    Serial.print("ADC: "); Serial.print(thermocouple.readADC() * 2); Serial.println(" uV");
    // Flush the serial buffer
    Serial.flush();
#else
#ifdef DEBUG_PRINT_CSV_DATA
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
#endif
#endif
}

static void readCmds(void) {
  using namespace REFLOW;
  static unsigned int idx = 0;
  if(Serial.available()) {
    char locChr = Serial.read();
    // Line feed or carriage return?
    if(locChr == 0x0A || locChr == 0x0D) {
      // Yes - Add null terminator and process command
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

static void processCmd(void) {
  using namespace REFLOW;

  // Process commands

  /// PRINT COMMANDS
  if(!strcmp(cmd, "help")) {
    printHelp();
  }
  /// TOGGLE RUN MODE
  else if(!strcmp(cmd, "debug")) {
    // Toggle debug mode
    if(_debugMode == true){
      _debugMode = false;
      
      Serial.println(">> debug mode disabled");
    }
    else {
      _debugMode = true;
      Serial.println(">> debug mode enabled");
      Serial.println("");
      printHelp();
    }
  }
  else if(!strcmp(cmd, "start")) {
    if(SM::getState() == SM::STATE_IDLE){
      SM::setStartSignal();
      Serial.println(">> running");
    }
  }
  else if(!strcmp(cmd, "stop")) {
    SM::setStartSignal();
    Serial.println(">> stopped");
  }
  /// FAN ON
  else if(!strcmp(cmd, "fan on")) {
    if(SM::getState() == SM::STATE_IDLE){
      SYSTEM::ssrFanON();
      Serial.println(">> fan on");
    }
  }
  /// FAN OFF
  else if(!strcmp(cmd, "fan off")) {
    if(SM::getState() == SM::STATE_IDLE){
      SYSTEM::ssrFanOFF();
      Serial.println(">> fan off");
    }
  }
  /// HEATER TOP ON
  else if(!strcmp(cmd, "heater top on")) {
    if(SM::getState() == SM::STATE_IDLE){
      SYSTEM::ssrHeaterTopON();
      Serial.println(">> heater top on");
    }
  }
  /// HEATER TOP OFF
  else if(!strcmp(cmd, "heater top off")) {
    if(SM::getState() == SM::STATE_IDLE){
      SYSTEM::ssrHeaterTopOFF();
      Serial.println(">> heater top off");
    }
  }
  /// HEATER BOTTOM ON
  else if(!strcmp(cmd, "heater bottom on")) {
    if(SM::getState() == SM::STATE_IDLE){
      SYSTEM::ssrHeaterBottomON();
      Serial.println(">> heater bottom on");
    }
  }
  /// HEATER BOTTOM OFF
  else if(!strcmp(cmd, "heater bottom off")) {
    if(SM::getState() == SM::STATE_IDLE){
      SYSTEM::ssrHeaterBottomOFF();
      Serial.println(">> heater bottom off");
    }
  }
  /// SET SETPOINT
  else if(!strcmp(cmd, "setpoint")) {
    dataEntry = DATA_PID_SETPOINT;
    Serial.print(">> enter temp setpoint: ");
  }
  /// SETS PID KP
  else if(_debugMode == true && !strcmp(cmd, "kp")) {
    dataEntry = DATA_PID_KP;
    Serial.print(">> enter kp: ");
  }
  /// SETS PID KI
  else if(_debugMode == true && !strcmp(cmd, "ki")) {
    dataEntry = DATA_PID_KI;
    Serial.print(">> enter ki: ");
  }
  /// SETS PID KD
  else if(_debugMode == true && !strcmp(cmd, "kd")) {
    dataEntry = DATA_PID_KD;
    Serial.print(">> enter kd: ");
  }
  /// GET TERMS
  else if(_debugMode == true && !strcmp(cmd, "get pid")) {
    Serial.print("PID: ");
    Serial.print(_kp);
    Serial.print(", ");
    Serial.print(_ki);
    Serial.print(", ");
    Serial.println(_kd);
  }
  /// GET TEMPERATURES
  else if(!strcmp(cmd, "get temps")) {
    Serial.print("TC: ");
    Serial.print(THERMOCOUPLE::getTemperature());
    Serial.print(", Amb: ");
    Serial.println(THERMOCOUPLE::getTemperatureAmbient());
  }
  else if(dataEntry != NUM_DATA_ENTRIES) {
    if(_debugMode) {
      processDataEntry();
    }
  }
  else {
    Serial.println(">> unknown command");
  }
}

static void processDataEntry(void) {
  using namespace REFLOW;
  String locStr = "";
  switch(dataEntry) {
    case DATA_PID_SETPOINT:
      locStr += cmd;
      _setpoint = locStr.toDouble();
      PID::setPID_setPoint(_setpoint);
      Serial.println();
      Serial.print(">> temp set to: ");
      Serial.println(_setpoint);
      break;

    case DATA_PID_KP:
      locStr += cmd;
      _kp = locStr.toDouble();
      PID::setPID_terms(_kp, _ki, _kd);
      Serial.println();
      Serial.print(">> kp set to: ");
      Serial.println(_kp);
      break;

    case DATA_PID_KI:
      locStr += cmd;
      _ki = locStr.toDouble();
      PID::setPID_terms(_kp, _ki, _kd);
      Serial.println();
      Serial.print(">> ki set to: ");
      Serial.println(_ki);
      break;

    case DATA_PID_KD:
      locStr += cmd;
      _kd = locStr.toDouble();
      PID::setPID_terms(_kp, _ki, _kd);
      Serial.println();
      Serial.print(">> kd set to: ");
      Serial.println(_kd);
      break;

    default:
      break;
  }
  // Reset data entry mode
  dataEntry = NUM_DATA_ENTRIES;
}

static void printHelp(void){
  Serial.println("Commands:");
  Serial.println(" debug");
  Serial.println(" start/stop");
  Serial.println(" fan on/off");
  Serial.println(" heater top on/off");
  Serial.println(" heater bottom on/off");
  if(_debugMode) {
    Serial.println(" setpoint");
    Serial.println(" kp");
    Serial.println(" ki");
    Serial.println(" kd");
    Serial.println(" get pid");
    Serial.println(" get temps");
    Serial.println("");
  }
}

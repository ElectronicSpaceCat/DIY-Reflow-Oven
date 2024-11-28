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

#ifndef _REFLOW_STATE_MACHINE_H
#define _REFLOW_STATE_MACHINE_H

#include "reflow_profiles.h"

namespace REFLOW::SM {

  typedef enum
  {
    STATE_PROFILE = 0,
    STATE_RESET,
    STATE_DEBUG,
    STATE_ABORT,
    STATE_IDLE,
    STATE_START,
    STATE_PREHEAT,
    STATE_SOAK,
    STATE_REFLOW,
    STATE_COOL,
    STATE_COMPLETE,
    STATE_HOT,
    STATE_ERROR
  } state_t;

  // Strings for each state
  extern const char *reflowStateStrings[];

  typedef enum
  {
    STATUS_OFF = 0,
    STATUS_ON
  } status_t;

  void init(void);
  void poll(void);
  state_t getState(void);
  status_t getStatus(void);
  zone_t getZone(void);
  unsigned long get_timerReflow(void);
  unsigned long get_timerZone(void);
  void setStartSignal(void);
  void setStopSignal(void);
}

#endif // _REFLOW_STATE_MACHINE_H

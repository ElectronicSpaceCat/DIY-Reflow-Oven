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
 * This file contains the reflow profiles. Each profile consists of a
 * name, temperature set points, time for each zone, P-term, I-term, D-term
 *
 * Modify them as needed. Every toaster over will have different temperature
 * profiles.
 *
 * While connected to a program like Putty, one can print/record the temperature values
 * over time using the debug interface over USB. This data can be plotted and analyzed to tune the PID terms.
 *
 * NOTE: The 'time' parameter is currently not used. The thought behind this parameter was to enforce a time limit
 *       based on a predefined profile with max zone times. Either cancel the reflow process if a single zone goes over time
 *       or use it to adjust following zone times. Tuning the profile PID well will get the times in the ball-park of what is
 *       required; therefore, the time parameter was ignored to finish the project sooner.
 */

#include "reflow_common.h"

namespace {
  byte _profileID = 0;
}

namespace REFLOW {
  /*
   * Structure of profile data:
   *
   * Example: LF1 (LF - Lead Free)
   *
   *            SET     SET
   * ZONE    |  TEMP(F) TIME(s) KP	 KI    KD
   * ==================================================
   * PREHEAT |  150     90      100  0.10  1500
   * SOAK    |  180     90      75   0.05  2200
   * REFLOW  |  210     60      100  0.10  1000
   * COOL    |  100     0       100  0.10  1000
   *
   */
  const profileData_t profileData[NUM_PROFILES] = {
      {
		{"LF1"},
		{{ { 115, 90 }, { 90,  0.10, 1500 } },
		 { { 145, 90 }, { 65,  0.05, 2200 } },
		 { { 175, 60 }, { 100, 0.10, 1000 } },
		 { {  50,  0 }, { 100, 0.10, 1000 } } }
      },
      {
		{"LF2"},
		{{ { 120, 90 }, { 100, 0.10, 1500 } },
		 { { 150, 90 }, { 75,  0.05, 2200 } },
		 { { 180, 60 }, { 100, 0.10, 1000 } },
		 { {  50,  0 }, { 100, 0.10, 1000 } } }
      },
      {
		{"LF3"},
		{{ { 125, 90 }, { 100, 0.10, 1500 } },
		 { { 155, 90 }, { 75,  0.05, 2200 } },
		 { { 185, 60 }, { 100, 0.10, 1000 } },
		 { {  50,  0 }, { 100, 0.10, 1000 } } }
      },
      {
		{"LF4"},
		{{ { 130, 90 }, { 100, 0.10, 1500 } },
		 { { 160, 90 }, { 75,  0.05, 2200 } },
		 { { 190, 60 }, { 100, 0.10, 1000 } },
		 { {  50,  0 }, { 100, 0.10, 1000 } } }
      },
      {
		{"LF5"}, // This is from the Kester Reflow Profile
		{{ { 150, 90 }, { 100, 0.10, 1500 } },
		 { { 180, 90 }, { 75,  0.05, 2200 } },
		 { { 210, 60 }, { 100, 0.10, 1000 } },
		 { { 100, 0  }, { 100, 0.10, 1000 } } }
      },
  };

  byte getProfileID(void){
    return _profileID;
  }

  void setProfileID(byte id){
    if(id < NUM_PROFILES){
      _profileID = id;
    }
  }

  const profileData_t* getCurrentProfile(void){
     return &profileData[_profileID];
  }

}

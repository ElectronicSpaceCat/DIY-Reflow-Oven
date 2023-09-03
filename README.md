# DIY-Reflow-Oven
Firmware for running a PID controlled reflow oven

The toaster oven used in this project was the Black & Decker TO1785SG.

I was inpsired to make this project when I came across the https://www.rocketscream.com/blog/product/tiny-reflow-controller-v2/ controller while looking for a simple solution to building an oven myself.

<img src="https://github.com/ElectronicSpaceCat/diy-reflow-oven/blob/master/extras/images/DSC_0013.JPG" width="250"> <img src="https://github.com/ElectronicSpaceCat/diy-reflow-oven/blob/master/extras/images/DSC_0017.JPG" width="250"> <img src="https://github.com/ElectronicSpaceCat/diy-reflow-oven/blob/master/extras/images/DSC_0014.JPG" width="250">

Unfortunately the controllers were out of stock at the time so I ordered some of the parts like the OLED screen and SSRs to design my own implementation.

I chose to use an Arduino Mega 2560 Rev3 that I had laying around (finally had a use for it) and I wrote the firmware in C/C++ in Eclipse. I used a makefile from a Nordic nRF52 project to build it (to be independent of any IDE used). Note: The makefile will need to be modified depending on where your source files are located.

This reflow oven design has a single button for accessing/selecting reflow profiles and returning to the main screen.

A few toaster build guides:  
https://www.rocketscream.com/blog/forums/topic/reflow-oven-builds/  
https://www.whizoo.com/reflowoven

## Tools Needed
* Dremel or something to cut holes in the toaster
* 3d printer for the face plates

## Parts Used
* 1x Arduino Mega 2560 Rev3 (could likely port to other versions)  
https://store.arduino.cc/products/arduino-mega-2560-rev3

* 1x TO1785SG Toaster  
https://www.amazon.com/BLACK-DECKER-TO1785SG-Toaster-4-Slice/dp/B08R7WCRT8/ref=cm_cr_arp_d_product_top?ie=UTF8

* 1x Adafruit MCP9600  
https://www.amazon.com/Adafruit-4101-MCP9600-Thermocouple-Amplifier/dp/B07SZ8YVTW/ref=sr_1_1?keywords=adafruit+mcp9600&sr=8-1

* 1x Alligator Clip K-Type Thermocouple Thermometer  
https://www.amazon.com/GenieFun-Alligator-Thermocouple-Thermometer-Temperature/dp/B01N11I90O

* 1x 6AN Weld on Bung Fitting (optional, but was used to run the thermocouple through the wall to the electronics side)  
https://www.amazon.com/EVIL-ENERGY-Fitting-Adapter-Stainless/dp/B089VP476L/ref=sr_1_6?keywords=weld%2Bbung%2Bsteel&sr=8-6&th=1

* 1x 7/16-20 Hex Jam Nut (for the weld bung)
https://www.amazon.com/16-20-Fine-Thread-Stainless-Steel/dp/B07QHFX87J/ref=sr_1_7?keywords=7%2F16-20+jam+nut&sr=8-7

* 1x Momentary Push Button Switch High Round Cap Waterproof Stainless Steel  
https://www.amazon.com/Waterproof-Momentary-Stainless-Button-Terminals/dp/B079HTQ7XD/ref=sr_1_6?keywords=push%2Bbutton%2Bswitch%2Bmetal&sr=8-6&th=1

* 1x OLED 128x64 Pixel  
https://www.amazon.com/Hosyond-Display-Self-Luminous-Compatible-Raspberry/dp/B09C5K91H7/ref=sr_1_4?keywords=oled%2Bdisplay&s=electronics&sr=1-4&th=1

* 3x SSR-40 AA (Kinda cheap (had a terminal break off), may want something better)  
1x each for the bottom element, top element, and fan (optional)  
https://www.amazon.com/SSR-40AA-80-250V-Output-24-380V-SSR-40/dp/B07FVDL7YX/ref=sr_1_11?keywords=solid%2Bstate%2Brelay&sr=8-11&th=1

* 9VDC 1A Arduino Compatible Power Supply Adapter 110V AC (barrel jack needs to be cut off)
https://www.amazon.com/dp/B018OLREG4?th=1&psc=1&language=en_US

* Quick Connects (These things are amazing)  
https://www.amazon.com/Connectors-Conductor-Combination-Assortment-Connection/dp/B09CKDWK4Q/ref=sr_1_96_sspa?keywords=lever%2Bwire%2Bconnectors&s=industrial&sr=1-96-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9idGY&th=1

* Reflect-A-GOLD   
https://www.designengineering.com/reflect-a-gold-heat-reflective-tape-2-x-15/

* Floor & Tunnel Shield II   
https://www.designengineering.com/floor-tunnel-shield-ii-21-x-24/

* Temperature Silicone Sealant   
https://www.amazon.co.uk/Weld-31314-Temperature-Silicone-Sealant/dp/B00ID8IUJY/ref=sr_1_10?keywords=heat+resistant+silicone+sealant&sr=8-10

## Arduino Pinout
Refere to: ``` ./src/reflow_system.h ```

## How to use
* A short press at the main screen will start the reflow cycle. The temperature will be graphed over time and a vertical marker will indicated when it's in the next zone.
* Long press the button at the main screen to access the profile selecton screen. Once there, quick presses will cycle 1 of 5 hardcoded profiles.
Profiles can be modified in: ``` ./src/reflow_common.cpp ```
* Long press again to return to the main screen.
* Short press anytime during a cycle will abort it

## Screen Legend
<img src="https://github.com/ElectronicSpaceCat/diy-reflow-oven/blob/master/extras/images/rf_complete.jpg" width="250">`  

From top left to bottom right (first line)  
<pre>
LF1       -  Lead Free Profile 1 (can modify but should keep it to 3 characters)  
Preheat   -  One of the states in: */src/reflow_state_machine.h >> state_t  
--.--C    -  Temperature read by the thermocouple  
</pre>

From top left to bottom right (second line)  
<pre>
R:--      -  Reflow time in seconds  
Z:--      -  Zone time in seconds  
S:---.--C -  Temperature setpoint for current zone
</pre>

## Compiling
#### Windows/Linux
* MinGW required for Windows to build, install instructions here: https://www.geeksforgeeks.org/installing-mingw-tools-for-c-c-and-changing-environment-variable/)
* Download the arm sdk to the project root folder from: https://developer.arm.com/downloads/-/gnu-rm
  * Windows: ```gcc-arm-none-eabi-10.3-2021.10-win32.zip```
  * Linux 32bit: ```gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2```
  * Linux 64bit: ```gcc-arm-none-eabi-10.3-2021.10-aarch64-linux.tar.bz2```
* Clone to project root folder: https://github.com/arduino/ArduinoCore-sam.git
* Clone to project root folder: https://github.com/arduino/arduino-flash-tools.git
* Open command terminal: ```cd (path location here)/diy-reflow-oven```
* Run ```make```

  #### Update the following paths if needed:
  * ```./Makefile```:
    ```
    SDK_ROOT := $(PROJ_DIR)/ArduinoCore-sam
    SDK_TOOLS_ROOT := $(PROJ_DIR)/ArduinoCore-sam/packages/arduino/tools
    LINKER_DIR := $(SDK_ROOT)/variants/arduino_due_x/linker_scripts/gcc
    GNU_INSTALL_ROOT := $(PROJ_DIR)/gcc-arm-none-eabi-10.3-2021.10/bin/
    ```

## Flashing Firmware
  #### Windows
  * Run ```./usb_flash.bat``` and make sure to replace ```COM4``` with the correct port at the line  ```COMM_PORT := COM4```
  * The executable location in ```./usb_flash.bat``` can be modified at the line:
    ```
    $(PROJ_DIR)\arduino-flash-tools\tools_windows\bossac\bin
    ```
  #### Linux
  * Make sure the script is executable with ```sudo chmod +x usb_flash.sh```
  * Run ```./usb_flash.sh``` and make sure to replace ```ttyACM1``` with the correct port at the line ```COMM_PORT=ttyACM1```
  * Typically the port should be on ```ttyACM0``` or ```ttyACM1```
  * The executable location in ```./usb_flash.sh``` can be modified at the line:
    ```
    $(PROJ_DIR)\arduino-flash-tools\tools_linux_64\bossac\bin
    ```
  #### Notes
  * Precompiled binaries are also available at ```./build```
  * If the Adruino binary drag and drop flash method isn't broken like mine was then you may try that option instead.

## Debug/Testing
* Use a program like Putty to connect to the Arduino over USB at 115200 baud rate.  
* Type "help" to get a list of available commands.
* PID Tuning: Replace reflow_main.cpp with pid_tuner.cpp and recompile. Type "help" for available commands.
* The PID tuner is separate because it wasn't meant for reflow purposes. It was for recording the temperature ramp rates from room temp to the setpoint. Sadly, I lost spreadsheet with some of my test data. However, one can set Putty to record the output window and save it as a csv file, which the debug output is already formatted for it.

## TODO
* schematic
* more images

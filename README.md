# DIY-Reflow-Oven
Firmware for running a PID controlled reflow oven

The toaster oven used in this project was the Black & Decker TO1785SG.

I was inpsired to make this project when I came across the https://www.rocketscream.com/blog/product/tiny-reflow-controller-v2/ controller while looking for a simple solution to building an oven myself.

Unfortunately the controllers were out of stock at the time so I ordered some of the parts like the OLED screen and SSRs and wrote firmware for it to suit my needs and to have more functionallity.

Instead of a custom board, I chose to use an Arduino Mega 2560 Rev3 that I had laying around (finally had a use for it).

Rather than using the Ardunino IDE and the .ino file type, I wrote this entirely in c/c++ using the Eclipse IDE and modified a makefile from a Nordic nRF52 project to build it (to be independent of any IDE used). Note: The makefile will need to be modified depending on where your source files are located.

This reflow oven design has a single button for accessing/selecting reflow profiles and returning to the main screen.

A few toaster build guides:

https://www.rocketscream.com/blog/forums/topic/reflow-oven-builds/

https://www.whizoo.com/reflowoven

#TODO
* schematics
* pictures

# DIY-Reflow-Oven
Firmware for running a PID controlled reflow oven.

The toaster oven used in this project was the Black & Decker TO1785SG.

I was inpsired to make this project when I came across the RocketScream tiny reflow controller
while looking for a simple solution to building an oven myself.

The controller:

https://www.rocketscream.com/blog/product/tiny-reflow-controller-v2/

https://github.com/rocketscream/TinyReflowController

Unfortunately the controllers were out of stock at the time so I ordered some of the parts
like the OLED screen and SSRs and completely rewrote the firmware to suit my needs and
and to have more functionallity.

Rather than using the Ardunino IDE and the .ino file type, I wrote this entirely in c/c++ using the
Eclipse IDE and modified a makefile from a Nordic nRF52 project to build it (to be independent of any IDE used).

This reflow oven design has a single button for accessing/selecting reflow profiles and returning to the main screen.

A few toaster build guides:

https://www.rocketscream.com/blog/forums/topic/reflow-oven-builds/

https://www.whizoo.com/reflowoven

(to be continued)

*schematics

*pictures

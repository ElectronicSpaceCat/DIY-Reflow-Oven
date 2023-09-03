#!/bin/bash

# This script is used to flash the reflow oven firmware on the Arduino Mega 2560 Rev3
# in the event the CMSIS DAP failed to work (ie the drag-drop firmware interface)

#Typically ttyACM0 or ttyACM1
COMM_PORT="ttyACM1"

#By setting the COM port to a different baud it effectively kills the arduino comms, allowing bossac.exe to talk to it

stty -F /dev/$COMM_PORT 1200

# Erase (-e), write (-w), verify (-v), set to boot from flash (-b), and reset (-R)

PROJ_DIR=.
BOSSAC_DIR=$PROJ_DIR/arduino-flash-tools/tools_linux_64/bossac/bin
BIN_FILE=$PROJ_DIR/build/diy_reflow_app.bin

$BOSSAC_DIR/bossac -p $COMM_PORT -U false -e -w -b $BIN_FILE -R

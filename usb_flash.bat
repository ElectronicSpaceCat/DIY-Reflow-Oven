ECHO OFF

::By setting the COM port to a different baud it effectively kills the arduino comms, allowing bossac.exe to talk to it

MODE COM4:1200,N,8,1,P

::Now erase (-e), write (-w), verify (-v), set to boot from flash (-b), and reset (-R)

PROJ_DIR   := ./
BOSSAC_DIR := $(PROJ_DIR)\Arduino15\packages\arduino\tools\bossac\1.6.1-arduino
COMM_PORT  := COM4
BIN_FILE   := $(PROJ_DIR)\diy_reflow_app.bin

$(BOSSAC_DIR)\bossac.exe --port=$(COMM_PORT) -U false -e -w -b $(BIN_FILE) -R

#PAUSE
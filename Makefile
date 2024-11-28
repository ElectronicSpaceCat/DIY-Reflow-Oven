PROJECT_NAME     := diy_reflow
TARGETS          := diy_reflow_app
OUTPUT_DIRECTORY := build

PROJ_DIR := ./
SDK_ROOT := $(PROJ_DIR)/ArduinoCore-sam
SDK_TOOLS_ROOT := $(PROJ_DIR)/ArduinoCore-sam/packages/arduino/tools
LINKER_DIR := $(SDK_ROOT)/variants/arduino_due_x/linker_scripts/gcc

GNU_INSTALL_ROOT := ./.$(PROJ_DIR)/gcc-arm-none-eabi-10.3-2021.10/bin/
GNU_VERSION := 10.3.1
GNU_PREFIX := arm-none-eabi

$(OUTPUT_DIRECTORY)/$(TARGETS).out: \
  LINKER_SCRIPT := $(LINKER_DIR)/flash.ld

# Source files common to all targets
SRC_FILES += \
	$(PROJ_DIR)/src/reflow_main.cpp \
	$(PROJ_DIR)/src/reflow_buttons.cpp \
	$(PROJ_DIR)/src/reflow_profiles.cpp \
	$(PROJ_DIR)/src/reflow_debug.cpp \
	$(PROJ_DIR)/src/reflow_led.cpp \
	$(PROJ_DIR)/src/reflow_oled.cpp \
	$(PROJ_DIR)/src/reflow_oled_gfx.cpp \
	$(PROJ_DIR)/src/reflow_pid.cpp \
	$(PROJ_DIR)/src/reflow_state_machine.cpp \
	$(PROJ_DIR)/src/reflow_system.cpp \
	$(PROJ_DIR)/src/reflow_thermocouple.cpp \
	$(PROJ_DIR)/libs/Adafruit_BusIO/Adafruit_BusIO_Register.cpp \
	$(PROJ_DIR)/libs/Adafruit_BusIO/Adafruit_I2CDevice.cpp \
	$(PROJ_DIR)/libs/Adafruit_BusIO/Adafruit_SPIDevice.cpp \
	$(PROJ_DIR)/libs/Adafruit_GFX_Library/glcdfont.c \
	$(PROJ_DIR)/libs/Adafruit_GFX_Library/Adafruit_GFX.cpp \
	$(PROJ_DIR)/libs/Adafruit_MCP9600_Library/Adafruit_MCP9600.cpp \
	$(PROJ_DIR)/libs/Adafruit_SSD1306/Adafruit_SSD1306.cpp \
	$(PROJ_DIR)/libs/Bounce2/src/Bounce2.cpp \
	$(PROJ_DIR)/libs/LiquidCrystal/src/LiquidCrystal.cpp \
	$(PROJ_DIR)/libs/PID/PID_v1.cpp \
	$(SDK_ROOT)/cores/arduino/main.cpp \
	$(SDK_ROOT)/cores/arduino/WInterrupts.c \
	$(SDK_ROOT)/cores/arduino/cortex_handlers.c \
	$(SDK_ROOT)/cores/arduino/hooks.c \
	$(SDK_ROOT)/cores/arduino/iar_calls_sam3.c \
	$(SDK_ROOT)/cores/arduino/itoa.c \
	$(SDK_ROOT)/cores/arduino/syscalls_sam3.c \
	$(SDK_ROOT)/cores/arduino/wiring.c \
	$(SDK_ROOT)/cores/arduino/wiring_analog.c \
	$(SDK_ROOT)/cores/arduino/wiring_digital.c \
	$(SDK_ROOT)/cores/arduino/wiring_shift.c \
	$(SDK_ROOT)/cores/arduino/IPAddress.cpp \
	$(SDK_ROOT)/cores/arduino/Print.cpp \
	$(SDK_ROOT)/cores/arduino/Reset.cpp \
	$(SDK_ROOT)/cores/arduino/RingBuffer.cpp \
	$(SDK_ROOT)/cores/arduino/Stream.cpp \
	$(SDK_ROOT)/cores/arduino/UARTClass.cpp \
	$(SDK_ROOT)/cores/arduino/USARTClass.cpp \
	$(SDK_ROOT)/cores/arduino/WMath.cpp \
	$(SDK_ROOT)/cores/arduino/WString.cpp \
	$(SDK_ROOT)/cores/arduino/abi.cpp \
	$(SDK_ROOT)/cores/arduino/new.cpp \
	$(SDK_ROOT)/cores/arduino/watchdog.cpp \
	$(SDK_ROOT)/cores/arduino/wiring_pulse.cpp \
	$(SDK_ROOT)/cores/arduino/wiring_pulse_asm.S \
	$(SDK_ROOT)/cores/arduino/avr/dtostrf.c \
	$(SDK_ROOT)/cores/arduino/USB/CDC.cpp \
	$(SDK_ROOT)/cores/arduino/USB/PluggableUSB.cpp \
	$(SDK_ROOT)/cores/arduino/USB/USBCore.cpp \
	$(SDK_ROOT)/variants/arduino_due_x/variant.cpp \
	$(SDK_ROOT)/system/libsam/source/adc.c \
	$(SDK_ROOT)/system/libsam/source/adc12_sam3u.c \
	$(SDK_ROOT)/system/libsam/source/can.c \
	$(SDK_ROOT)/system/libsam/source/dacc.c \
	$(SDK_ROOT)/system/libsam/source/efc.c \
	$(SDK_ROOT)/system/libsam/source/emac.c \
	$(SDK_ROOT)/system/libsam/source/gpbr.c \
	$(SDK_ROOT)/system/libsam/source/interrupt_sam_nvic.c \
	$(SDK_ROOT)/system/libsam/source/pio.c \
	$(SDK_ROOT)/system/libsam/source/pmc.c \
	$(SDK_ROOT)/system/libsam/source/pwmc.c \
	$(SDK_ROOT)/system/libsam/source/rstc.c \
	$(SDK_ROOT)/system/libsam/source/rtc.c \
	$(SDK_ROOT)/system/libsam/source/rtt.c \
	$(SDK_ROOT)/system/libsam/source/spi.c \
	$(SDK_ROOT)/system/libsam/source/ssc.c \
	$(SDK_ROOT)/system/libsam/source/tc.c \
	$(SDK_ROOT)/system/libsam/source/timetick.c \
	$(SDK_ROOT)/system/libsam/source/trng.c \
	$(SDK_ROOT)/system/libsam/source/twi.c \
	$(SDK_ROOT)/system/libsam/source/udp.c \
	$(SDK_ROOT)/system/libsam/source/udphs.c \
	$(SDK_ROOT)/system/libsam/source/uotghs.c \
	$(SDK_ROOT)/system/libsam/source/uotghs_device.c \
	$(SDK_ROOT)/system/libsam/source/uotghs_host.c \
	$(SDK_ROOT)/system/libsam/source/usart.c \
	$(SDK_ROOT)/system/libsam/source/wdt.c \
	$(SDK_ROOT)/system/CMSIS/Device/ATMEL/sam3xa/source/system_sam3xa.c \
	$(SDK_ROOT)/system/CMSIS/Device/ATMEL/sam3xa/source/gcc_arm/startup_sam3xa.c \
	$(SDK_ROOT)/libraries/Wire/src/Wire.cpp \
	$(SDK_ROOT)/libraries/SPI/src/SPI.cpp \

# Include folders common to all targets
INC_FOLDERS += \
	src \
	$(PROJ_DIR)/libs/Adafruit_BusIO \
	$(PROJ_DIR)/libs/Adafruit_GFX_Library \
	$(PROJ_DIR)/libs/Adafruit_MCP9600_Library \
	$(PROJ_DIR)/libs/Adafruit_SSD1306 \
	$(PROJ_DIR)/libs/Bounce2 \
	$(PROJ_DIR)/libs/Bounce2/src \
	$(PROJ_DIR)/libs/LiquidCrystal/src \
	$(PROJ_DIR)/libs/PID \
	$(SDK_ROOT)/cores/arduino \
	$(SDK_ROOT)/cores/arduino/avr \
	$(SDK_ROOT)/cores/arduino/USB \
	$(SDK_ROOT)/libraries/Wire/src \
	$(SDK_ROOT)/libraries/SPI/src \
	$(SDK_ROOT)/system/libsam \
	$(SDK_ROOT)/system/libsam/source \
	$(SDK_ROOT)/system/libsam/include \
	$(SDK_ROOT)/variants/arduino_due_x \
	$(SDK_ROOT)/system/CMSIS/CMSIS/Include \
	$(SDK_ROOT)/system/CMSIS/Device/ATMEL \
	$(SDK_ROOT)/system/CMSIS/Device/ATMEL/sam3xa \
	$(SDK_ROOT)/system/CMSIS/Device/ATMEL/sam3xa/include \
	$(SDK_ROOT)/system/CMSIS/Device/ATMEL/sam3xa/include/pio \
	$(SDK_ROOT)/system/CMSIS/Device/ATMEL/sam3xa/include/component \
	$(SDK_ROOT)/system/CMSIS/Device/ATMEL/sam3xa/include/instance \
	$(SDK_ROOT)/system/CMSIS/Device/ATMEL/sam3xa/source \
	$(SDK_ROOT)/system/CMSIS/Device/ATMEL/sam3xa/source/gcc_arm \
	#$(SDK_TOOLS_ROOT)/arm-none-eabi-gcc/4.8.3-2014q1/share/gcc-arm-none-eabi/samples/startup \

# Libraries common to all targets
LIB_FILES += \

# Optimization flags
OPT = -Os -g3
# Uncomment the line below to enable link time optimization
#OPT += -flto

# Assembler flags common to all targets
ASMFLAGS += $(OPT)
ASMFLAGS += -DF_CPU=84000000L
ASMFLAGS += -DARDUINO=10813
ASMFLAGS += -DARDUINO_SAM_DUE
ASMFLAGS += -DARDUINO_ARCH_SAM
ASMFLAGS += -D__SAM3X8E__
ASMFLAGS += -DUSB_VID=0x2341
ASMFLAGS += -DUSB_PID=0x003e
ASMFLAGS += -DUSBCON
ASMFLAGS += -mcpu=cortex-m3
ASMFLAGS += -mthumb -mabi=aapcs

# C flags common to all targets
CFLAGS += $(ASMFLAGS)
CFLAGS += -Wall
# keep every function in a separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin -fshort-enums

# C++ flags common to all targets
CXXFLAGS += $(CFLAGS)
CXXFLAGS += -fno-exceptions -fno-rtti -fno-threadsafe-statics

# Linker flags
LDFLAGS += $(OPT)
LDFLAGS += -mthumb -mabi=aapcs -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m3
# let linker dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs

# $(TARGETS): CFLAGS += -D__HEAP_SIZE=8192
# $(TARGETS): CFLAGS += -D__STACK_SIZE=8192
# $(TARGETS): ASMFLAGS += -D__HEAP_SIZE=8192
# $(TARGETS): ASMFLAGS += -D__STACK_SIZE=8192

# Add standard libraries at the very end of the linker input, after all objects
# that may need symbols provided by these libraries.
LIB_FILES += -lc -lnosys -lm

.PHONY: default help

# Default target - first one defined
default: $(TARGETS)

TEMPLATE_PATH := $(PROJ_DIR)/scripts

include $(TEMPLATE_PATH)/Makefile.common

$(foreach target, $(TARGETS), $(call define_target, $(target)))

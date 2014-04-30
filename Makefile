include ../../build/variables.mk

PROJECT=stubby
MMCU=atmega1284p
#COMPILER=avr-g++
F_CPU=10000000
SOURCES=stubby.c comm.c gait.c tripod.c leg.c servo.c lib/pwm/pwm.c lib/serial/serial.c lib/serial/serial_sync_tx.c
AVRDUDE_PROGRAMMER=usbtiny

# You can also define anything here and it will override 
# the definitions in variables.mk

CDEFS+=-DPWM_MAX_PINS=18 -DPWM_PORTD_UNUSED

HFUSE=0xD9
LFUSE=0xFF

include ../../build/targets.mk

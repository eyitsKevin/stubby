#ifndef STUBBY_H
#define STUBBY_H

#ifndef DEBUG_SIMULATION
#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#endif

#define LEG_COUNT		6

#define FRONT_LEFT		0
#define FRONT_RIGHT		1
#define MIDDLE_LEFT		2
#define MIDDLE_RIGHT	3
#define REAR_LEFT		4
#define REAR_RIGHT		5

#define JOINT_COUNT		3

#define TIBIA			0
#define FEMUR			1
#define COXA			2

#define LED_RED 		(LEG_COUNT * JOINT_COUNT) + 0
#define LED_GREEN 		(LEG_COUNT * JOINT_COUNT) + 1
#define LED_BLUE 		(LEG_COUNT * JOINT_COUNT) + 2

#define PWM_COUNT		(LEG_COUNT * JOINT_COUNT) + 3


#ifndef DEBUG_SIMULATION
#include "status.h"
#include "lib/serial/serial.h"
#include "comm.h"
#endif

#include "Leg.h"
#endif

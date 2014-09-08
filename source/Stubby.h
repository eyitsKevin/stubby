#ifndef STUBBY_H
#define STUBBY_H

#include <avr/io.h>
#include <avr/wdt.h>
#include <stdint.h>

#include "lib/protocol/protocol.h"
#include "lib/universal_controller/client.h"
#include "lib/serial/serial.h"
#include "lib/pwm/pwm.h"

#define PCB_REVISION		1

#define LEG_COUNT			6

#define FRONT_LEFT			0
#define MIDDLE_LEFT			1
#define REAR_LEFT			2
#define REAR_RIGHT			3
#define MIDDLE_RIGHT		4
#define FRONT_RIGHT			5

#define JOINT_COUNT			3

#define TIBIA				0
#define FEMUR				1
#define COXA				2

#define LED_RED 			((LEG_COUNT * JOINT_COUNT) + 0)
#define LED_GREEN 			((LEG_COUNT * JOINT_COUNT) + 1)
#define LED_BLUE 			((LEG_COUNT * JOINT_COUNT) + 2)

#define PWM_COUNT			((LEG_COUNT * JOINT_COUNT) + 3)

#define CALIBRATION_COUNT	(JOINT_COUNT + 3)
#define CALIBRATION_X		(JOINT_COUNT + 0)
#define CALIBRATION_Y		(JOINT_COUNT + 1)
#define CALIBRATION_Z		(JOINT_COUNT + 2)

//State variables
#define POWER_OFF				0x00
#define POWER_ON				0x01

#define CONTROLLER_NONE			0x00
#define CONTROLLER_UC			0x01
#define CONTROLLER_PROCESSING	0x02


void servo_init();

uint8_t get_power();
void set_power(uint8_t power);

uint8_t get_controller();

uint8_t doMove(double linear_angle, double linear_velocity, double rotational_velocity);
void doTranslate(uint16_t x, uint16_t y, uint16_t z);
void doRotate(double pitch, double roll, double yaw);

void mode_select();
void mode_remote_control();
void mode_calibration();
	
#endif

#include "processing.h"

uint8_t movement_required = 0x00;
uint16_t desired_distance;
double desired_linear_angle;
double desired_linear_velocity;
double desired_rotational_angle;
double desired_rotational_velocity;

void processing_command_executor(){
	wdt_reset();
	
	if (movement_required){
		uint8_t step_distance = doMove(desired_linear_angle, desired_linear_velocity, desired_rotational_velocity);
		if (step_distance <= desired_distance){
			desired_distance -= step_distance;
		}
		else {
			desired_distance = 0;
		}
	}
}

void processing_dispatch_message(uint8_t cmd, uint8_t *message, uint8_t length){
	if (cmd == MESSAGE_REQUEST_POWER_ON){
		set_power(POWER_ON);
	}
	else if (cmd == MESSAGE_REQUEST_POWER_OFF){
		set_power(POWER_OFF);
	}
	else if (cmd == MESSAGE_REQUEST_MOVE){
		if (length == 2){
			movement_required = 0x01;
			desired_linear_angle = convert_byte_to_radian(message[0]);
			desired_rotational_angle = convert_byte_to_radian(message[1]);
			desired_rotational_velocity = 0.0;		//TODO 
			desired_linear_velocity = 1.0;
			desired_distance = 0xFFFF;
		}
		else if (length == 3){
			movement_required = 0x01;
			desired_linear_angle = convert_byte_to_radian(message[0]);
			desired_rotational_angle = convert_byte_to_radian(message[1]);
			desired_rotational_velocity = 0.0;		//TODO 
			desired_linear_velocity = message[2] / 256.0;
			desired_distance = 0xFFFF;
		}
		else if (length == 5){
			movement_required = 0x01;
			desired_linear_angle = convert_byte_to_radian(message[0]);
			desired_rotational_angle = convert_byte_to_radian(message[1]);
			desired_rotational_velocity = 0.0;		//TODO 
			desired_linear_velocity = message[2] / 256.0;
			desired_distance = message[3] << 8 | message[4];	
		}
	}
}
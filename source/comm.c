#include "comm.h"

static volatile double _velocity = 0; 	//0 is stopped, 1 is full speed ahead
static volatile double _direction = 0;	//0 is straight ahead, -1 is full left, 1 is full right
static volatile uint16_t _buttons = 0x00;	//Bit mask of any buttons pressed.  Bits are cleared when read.

static volatile uint8_t repeat_counter = 0x00;

void comm_init(){
	serial_init_b(38400);
}

uint16_t comm_read_buttons(){
	if (_buttons == 0x00) return 0x00;
	
	uint16_t buttons = _buttons;
	_buttons = 0x00;
	return buttons;
}

void comm_read_vector(double *velocity, double *direction){
	//Whenever this gets to a certain value, we invalidate all current messages.  On receipt of new message, we reset to 0.
	repeat_counter++;
	if (repeat_counter >= 0x0F){
		_velocity = 0;
		_direction = 0;
		repeat_counter = 0x00;
	}
	
	//Every few seconds we will send these controller init messages.
	static uint8_t controller_init = 0x00;		//Whenever this resets to 0, we re-send controler init messages
	controller_init++;
	if (controller_init >= 0xFF){
		serial_write_b(0x41);	//Enable analog sticks
		serial_write_b(0xFF);	//Set an analog repeat time of about 32ms.
		controller_init = 0x00;
	}
	

	
	*velocity = _velocity;
	*direction = _direction;
}

void _serial_init_rx(){
	//Enable RX interrupts
	UCSR0B |= _BV(RXCIE0);
	
	//Enable interrupts
	sei();
}

ISR(USART0_RX_vect){
	uint8_t b = UDR0;

	UCSR0B &= ~_BV(RXCIE0);	//Disable RX interrupts; we want PWM interrupts, but not RX interrupts.
	sei();	//We want PWM to continue uninterrupted while processing serial data

	if ((b & CONTROLLER_MESSAGE_TYPE_MASK) == CONTROLLER_MESSAGE_TYPE_ANALOG){	//Analog stick event
		if ((b & CONTROLLER_ANALOG_STICK) == CONTROLLER_ANALOG_STICK_LEFT){	//Left stick
			uint8_t value = b & CONTROLLER_ANALOG_VALUE_MASK;
			if ((b & CONTROLLER_ANALOG_AXIS) == CONTROLLER_ANALOG_AXIS_X){	//X Axis
				//The PSX controller sends X=0 for all the way left; we want to translate this to -1 for left.
				_direction = 1.0/15 * value - 1;
			}
			else if ((b & CONTROLLER_ANALOG_AXIS) == CONTROLLER_ANALOG_AXIS_Y){	//Y Axis
				//The PSX controller sends X=0 for all the way forward; we want to translate this to 1 for forward.
				_velocity = -1.0/15 * value + 1;
			}
		}
	}
	else {	//Digital button event
		if ((b & CONTROLLER_BUTTON_PRESS_MASK) == CONTROLLER_BUTTON_PRESS){
			_buttons |= _BV((b & CONTROLLER_BUTTON_VALUE_MASK));	//Set the button; this is cleared when they are read.
		}
	}
	
	repeat_counter = 0x00;
	//Re-enable RX interrupts
	UCSR0B |= _BV(RXCIE0);
}

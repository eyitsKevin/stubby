#ifndef HEX_RIPPLE
#define HEX_RIPPLE

#define DELAY 		40
#define MAX_COXA_STEP	0.3
#define TIBIA_STEP	1.5

#include <avr/io.h>

#include "comm.h"
#include "leg.h"

void ripple_step();

void ripple_gait_init();

#endif
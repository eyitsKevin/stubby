#ifndef POINT_H
#define POINT_H

#ifndef DEBUG_SIMULATION
#include <avr/io.h>
#else
#include "../simulation/debug.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#endif
#include "trig.h"


class Point {
	public:
		Point();
		Point(int16_t x, int16_t y, int16_t z);
		
		/*
		 * Adds the specified offset point to this point.
		 */
		void add(Point offset);

		/*
		 * Subtracts the specified offset point from this point.
		 */
		void subtract(Point offset);
		
		/*
		 * Rotate this point by specified angle on the x,y plane 
		 * around 0,0 (z component is ignored)
		 */
		void rotateXY(double angle);
		
		/*
		 * Rotate this point by specified angle on the x,y plane around 
		 * the specified origin (z component is ignored)
		 */
		void rotateXY(Point origin, double angle);

		/*
		 * Rotate this point by the specified angle around 
		 * the axis intercepting points 0,0,0 and v.
		 */
		void rotateXYZ(Point v, double angle);

		int16_t x;
		int16_t y;
		int16_t z;
};

#endif

// Color.h

#ifndef _COLOR_h
#define _COLOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Color
{
 public:
	uint8_t r;
	uint8_t g;
	uint8_t b;
	float intensity;
	
	Color();
	Color(uint8_t r, uint8_t g, uint8_t b, float intensity);
};


#endif


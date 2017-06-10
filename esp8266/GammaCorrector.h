// GammaCorrector.h

#ifndef _GAMMACORRECTOR_h
#define _GAMMACORRECTOR_h

#define GAMMA           2.8
#define MAX_IN          255.0
#define MAX_OUT         1023.0

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class GammaCorrector
{
 protected:
	float gamma;
	float maxIn;
	float maxOut;
	
	uint8_t gammaCorrectOneColor(uint8_t input); 
	
	public:
	GammaCorrector(float gamma, float maxIn, float maxOut);
	Color gammaCorrect(Color& input);
};


#endif


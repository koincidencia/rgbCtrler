// 
// 
// 

#include "GammaCorrector.h"
#include "math.h"


GammaCorrector::GammaCorrector(float gamma, float maxIn, float maxOut){
	this->gamma = gamma;
	this->maxIn = maxIn;
	this->maxOut = maxOut;
}

Color GammaCorrector::gammaCorrect(Color& input){
	Color output = new Color();
	output.r = gammaCorrectOneColor(input.r) * input.intensity;
	output.g = gammaCorrectOneColor(input.g) * input.intensity;
	output.b = gammaCorrectOneColor(input.b) * input.intensity;
	output.intensity = -1.0;
}

uint8_t GammaCorrector::gammaCorrectOneColor(uint8_t input){
	float tmp = ((float)(input))/((float)(maxIn));
	tmp = pow(tmp, gamma);
	tmp = tmp * maxOut + 0.5;
	return tmp;
}


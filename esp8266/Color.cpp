// 
// 
// 

#include "Color.h"

Color::Color(){
	r = g = b = 0;
	intensity = 0.0;
}

Color::Color(uint8_t r, uint8_t g, uint8_t b, float intensity){
	this->r = r;
	this->g = g;
	this->b = b;
	this->intensity = intensity;
}


#ifndef RGBCONTROLLER_H
#define RGBCONTROLLER_H

#include <Arduino.h>

//#define DEBUG_SERIAL

#define PIN_R           14
#define PIN_G           12
#define PIN_B           13

#define GAMMA           2.8
#define MAX_IN          255.0
#define MAX_OUT         1023.0 

#define PWM_FREQ		100

//DATA STRUCTURES
struct{
  unsigned int r;
  unsigned int g;
  unsigned int b;
  float intensity;
}rgbStruct;

//FUNCTIONS
void rgbControllerInit();
unsigned int GammaCorrection(unsigned int sRGB);
void SetRGB(uint8_t r, uint8_t g, uint8_t b);
void SetIntensity(float intensity);
void SetPwm();


#endif
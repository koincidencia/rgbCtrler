#include "rgbController.h"

void rgbControllerInit()
{
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  analogWriteFreq(PWM_FREQ);
  
  rgbStruct.r = 0;
  rgbStruct.g = 0;
  rgbStruct.b = 0;
  rgbStruct.intensity = 0.0f;
  
  SetPwm();
}

unsigned int GammaCorrection(unsigned int sRGB)
{
  float tmp = ((float)(sRGB))/((float)(MAX_IN));
  tmp = pow(tmp, GAMMA);
  tmp = tmp * MAX_OUT + 0.5;
  return tmp;
}

void SetRGB(uint8_t r, uint8_t g, uint8_t b)
{
	rgbStruct.r = r;
	rgbStruct.g = g;
	rgbStruct.b = b;
}

void SetIntensity(float intensity)
{
	rgbStruct.intensity = intensity;
}

void SetPwm()
{
  unsigned int r = GammaCorrection(rgbStruct.r)*rgbStruct.intensity;
  unsigned int g = GammaCorrection(rgbStruct.g)*rgbStruct.intensity;
  unsigned int b = GammaCorrection(rgbStruct.b)*rgbStruct.intensity;
  analogWrite(PIN_R, r);
  analogWrite(PIN_G, g);
  analogWrite(PIN_B, b);
  
#ifdef DEBUG_SERIAL
  Serial.printf("r=%d, g=%d, b=%d, i=%d\r\n", rgbStruct.r, rgbStruct.g, rgbStruct.b, (int)(rgbStruct.intensity*100));
  Serial.printf("r=%d, g=%d, b=%d\r\n", r, g, b);
#endif
}

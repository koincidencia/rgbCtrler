#include "Color.h"
#include "GammaCorrector.h"
#include "udpBroadcaster.h"
#include <Arduino.h>
#include "webService.h"

#define BAUDRATE	115200


void setup()
{
  Serial.begin(BAUDRATE);
  
  WebServiceInit();
  
  Serial.println("Init finished.");
  Serial.println();
}

void loop()
{
  WebServiceHandler();
}

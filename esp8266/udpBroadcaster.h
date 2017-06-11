// udpBroadcaster.h

#ifndef _UDPBROADCASTER_h
#define _UDPBROADCASTER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

class UDPBroadcaster{
	private:
		static const uint16_t port = 2390;
		WiFiUDP udp;
		static const int packetBufferLength = 256;
		static const int ipBufferLength = 256;
		char packetBuffer[packetBufferLength];
		char ipBuffer[ipBufferLength];
		
	public:	
		UDPBroadcaster();
		void BroadcastHandler(String hostname);
};


#endif


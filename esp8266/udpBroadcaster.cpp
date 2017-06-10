#include "udpBroadcaster.h"

UDPBroadcaster::UDPBroadcaster()
{
	udp.begin(port);
}

void UDPBroadcaster::BroadcastHandler(String hostname)
{
	int packetSize = udp.parsePacket();
	if(packetSize)
	{
		Serial.print("Received packet of size ");
		Serial.println(packetSize);
		Serial.print("From ");
		IPAddress remoteIp = udp.remoteIP();
		Serial.print(remoteIp);
		Serial.print(", port ");
		Serial.println(udp.remotePort());

		// read the packet into packetBufffer
		int len = udp.read(packetBuffer, packetBufferLength-1);
		if (len > 0) {
			packetBuffer[len] = 0;
		}
		Serial.println("Contents:");
		Serial.println(packetBuffer);
		
		uint16_t destPort = String(packetBuffer).toInt();
		
		// send a reply, to the IP address and port that sent us the packet we received
		String str = "NyihahaMuhaha-" + WiFi.localIP().toString() + "-" + hostname;
		Serial.println("Sending back ip info: ");
		Serial.println(str);
		str.toCharArray(ipBuffer, ipBufferLength);
		udp.beginPacket(udp.remoteIP(), destPort);
		udp.write(ipBuffer, str.length());
		udp.endPacket();
	}
}
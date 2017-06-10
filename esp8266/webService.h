#ifndef WEBSERVICE_H
#define WEBSERVICE_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "rgbController.h"

#define LOAD_FROM_EEPROM

#define INTERRUPT_PIN			0
#define PIN_CHECK_MS			2000
//#define USE_INTERRUPT

#define DEFAULT_SSID			"ZTEGiga"
#define DEFAULT_PASSWORD		"r4ekkorn"
#define DEFAULT_HOSTNAME		"rgbCtrler"
#define DEFAULT_IP				192,168,0,1
#define DEFAULT_GATEWAY			192,168,0,1
#define DEFAULT_SUBNET			255,255,255,0

#define HTTP_OK					200

#define ID_SSID					"ssid"
#define ID_PASSWORD				"password"
#define ID_HOSTNAME				"hostname"
#define ID_MODE					"mode"
#define ID_AP					"ap"
#define ID_STA					"sta"
#define ID_R					"r"
#define ID_G					"g"
#define ID_B					"b"
#define ID_INTENSITY			"intensity"

#define ID_REQUEST				"request"
#define ID_REQ_HOSTNAME			"hostname"
#define ID_REQ_IP				"ip"
#define ID_REQ_RSSI				"rssi"

#define SERVER_PORT       	    80
#define SERVER_URL_RGB    	    "/rgb"
#define SERVER_URL_SETUP  		"/setup"
#define SERVER_URL_REQUEST		"/request"
#define SERVER_URL_DISCOVERY    "/discovery"

#define LENGTH_SSID					32
#define LENGTH_PASSWORD				64
#define LENGTH_HOSTNAME				32

#define EEPROM_SIZE					256
#define EEPROM_ADDR_SSID_START		0
#define EEPROM_ADDR_PASSWD_START	100
#define EEPROM_ADDR_HOSTNAME_START	200
#define EEPROM_ADDR_MODE			255

#define REQ_ARGS        			4

#define AP_MODE						0x55
#define STA_MODE					0xAA

void WebServiceInit();
void WebServiceHandler();

#endif

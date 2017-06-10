#include "webService.h"
#include <IPAddress.h>
#include <String>
#include <EEPROM.h>
#include <ESP8266mDNS.h>
#include "udpBroadcaster.h"

void ConnectToWifiAP();
void SetupAP();
void StartWebServer();
String IpToString(IPAddress addr);
String LoadStringFromEEPROM(uint16_t eepromAddr, uint8_t length);
void WriteStringToEEPROM(uint16_t eepromAddr, String& str);
void InitializeStaticVariables();
void GPIOInterruptCallback();
void WebServiceInit();
void WebServiceHandler();
void WebServerSetupCallback();
void WebServerRequestCallback();
void WebServerDiscoveryCallback();
void WebServerRGBCallback();
void CheckUserButton();


static ESP8266WebServer httpServer(SERVER_PORT);
static String ip;
static uint8_t mode = 0;
static UDPBroadcaster broadcaster;

#ifndef LOAD_FROM_EEPROM
	static String ssid = DEFAULT_SSID;
	static String password = DEFAULT_PASSWORD;
	static String hostname = DEFAULT_HOSTNAME;
#else
	static String ssid;
	static String password;
	static String hostname;
#endif

void ConnectToWifiAP()
{
	Serial.println("Connecting to " + ssid);
	WiFi.mode(WIFI_STA);
	
	char ssid_ch[LENGTH_SSID];
	char password_ch[LENGTH_PASSWORD];
	ssid.toCharArray(ssid_ch, LENGTH_SSID);
	password.toCharArray(password_ch, LENGTH_PASSWORD);
	WiFi.begin(ssid_ch, password_ch);
	
	if(WiFi.waitForConnectResult() != WL_CONNECTED)
	{
		Serial.println("WiFi Connect Failed! Rebooting...");
		delay(1000);
		ESP.restart();
	}
	Serial.print("Connected to ");
	Serial.println(ssid);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
}

void SetupAP()
{
	IPAddress ip = IPAddress(DEFAULT_IP);
	IPAddress gateway = IPAddress(DEFAULT_GATEWAY);
	IPAddress subnet = IPAddress(DEFAULT_SUBNET);
	Serial.println("Setting up softAP as " + hostname);
	char hostname_ch[LENGTH_SSID];
	hostname.toCharArray(hostname_ch, LENGTH_HOSTNAME);
	WiFi.softAP(hostname_ch);
	WiFi.softAPConfig(ip, gateway, subnet);
}

void StartWebServer()
{
	httpServer.on(SERVER_URL_RGB, WebServerRGBCallback);
	httpServer.on(SERVER_URL_SETUP, WebServerSetupCallback);
	httpServer.on(SERVER_URL_DISCOVERY, WebServerDiscoveryCallback);
	httpServer.on(SERVER_URL_REQUEST, WebServerRequestCallback);
	httpServer.begin();
	Serial.println("Webserver started.");
}

String IpToString(IPAddress addr)
{
	String str;
	str.concat(addr[0]);
	str.concat('.');
	str.concat(addr[1]);
	str.concat('.');
	str.concat(addr[2]);
	str.concat('.');
	str.concat(addr[3]);
	return str;
}

String LoadStringFromEEPROM(uint16_t eepromAddr, uint8_t length)
{
	char character;
	String str;
	for(uint8_t k = 0; k < length; k++)
	{
		character = EEPROM.read(eepromAddr + k);
		if(character == '\0')
		{
			break;
		}
		else
		{
			str += character;
		}
	}
	return str;
}

void WriteStringToEEPROM(uint16_t eepromAddr, String& str)
{
	uint8_t k;
	for(k = 0; k < str.length(); k++)
	{
		EEPROM.write(eepromAddr + k, str[k]);
	}
	EEPROM.write(eepromAddr + k, '\0');
	EEPROM.commit();
}

void InitializeStaticVariables()
{
	EEPROM.begin(EEPROM_SIZE);
#ifdef LOAD_FROM_EEPROM
	ssid = LoadStringFromEEPROM(EEPROM_ADDR_SSID_START, LENGTH_SSID);
	password = LoadStringFromEEPROM(EEPROM_ADDR_PASSWD_START, LENGTH_PASSWORD);
	hostname = LoadStringFromEEPROM(EEPROM_ADDR_HOSTNAME_START, LENGTH_HOSTNAME);
	Serial.println("Default values were loaded from EEPROM.");
#else
	WriteStringToEEPROM(EEPROM_ADDR_SSID_START, ssid);
	WriteStringToEEPROM(EEPROM_ADDR_PASSWD_START, password);
	WriteStringToEEPROM(EEPROM_ADDR_HOSTNAME_START, hostname);
	delay(100);
	Serial.println("Default values were saved to EEPROM.");
#endif

	mode = EEPROM.read(EEPROM_ADDR_MODE);
}

void GPIOInterruptCallback()
{
	delay(2000);
	if(digitalRead(INTERRUPT_PIN) == LOW)
	{
		Serial.println("Setting mode back to AP mode.");
		EEPROM.write(EEPROM_ADDR_MODE, AP_MODE);
		EEPROM.commit();
		delay(1000);
		Serial.println("Rebooting...");
		ESP.restart();
	}
}

void WebServiceInit()
{
	pinMode(INTERRUPT_PIN, INPUT_PULLUP);
#ifdef USE_INTERRUPT
	attachInterrupt(INTERRUPT_PIN, GPIOInterruptCallback, FALLING);
#endif
	InitializeStaticVariables();
	rgbControllerInit();
	
	delay(1000);
	CheckUserButton();
	
	if(mode == AP_MODE)
	{
		SetupAP();
	}
	else
	{
		ConnectToWifiAP();
	}
	
	StartWebServer();
	ip = IpToString(WiFi.localIP());
	
	if (!MDNS.begin(hostname.c_str())) {
		Serial.println("Error setting up MDNS responder!");
	}
	else{
		Serial.println("MDNS responder initialized.");
	}
	MDNS.addService("http", "tcp", 80);
}

void WebServiceHandler()
{
	httpServer.handleClient(); //this is required for handling the incoming requests
	broadcaster.BroadcastHandler(hostname);
#ifndef USE_INTERRUPT
	CheckUserButton();
#endif
	delay(1);
}

void WebServerSetupCallback()
{
	String msg;
	bool reboot = false;
	
	Serial.println("Setup callback.");
	
	//STA SSID
	if(httpServer.arg(ID_SSID).length() > 0)
	{
		msg += ID_SSID;
		msg += " setup<br>";
		ssid = httpServer.arg(ID_SSID);
		WriteStringToEEPROM(EEPROM_ADDR_SSID_START, ssid);
	}
	//STA PASSWORD
	if(httpServer.arg(ID_PASSWORD).length() > 0)
	{
		msg += ID_PASSWORD;
		msg += " setup<br>";
		password = httpServer.arg(ID_PASSWORD);
		WriteStringToEEPROM(EEPROM_ADDR_PASSWD_START, password);
	}
	//HOSTNAME
	if(httpServer.arg(ID_HOSTNAME).length() > 0)
	{
		msg += ID_HOSTNAME;
		msg += " setup<br>";
		hostname = httpServer.arg(ID_HOSTNAME);
		WriteStringToEEPROM(EEPROM_ADDR_HOSTNAME_START, hostname);
	}
	//MODE
	if(httpServer.arg(ID_MODE).length() > 0)
	{
		if(httpServer.arg(ID_MODE) == ID_AP)
		{
			msg += ID_MODE;
			msg += " setup<br>";
			EEPROM.write(EEPROM_ADDR_MODE, AP_MODE);
			EEPROM.commit();
			reboot = true;
		}
		else if(httpServer.arg(ID_MODE) == ID_STA)
		{
			msg += ID_MODE;
			msg += " setup<br>";
			EEPROM.write(EEPROM_ADDR_MODE, STA_MODE);
			EEPROM.commit();
			reboot = true;
		}
		else
		{
			msg += ID_MODE;
			msg += " bad request<br>";
		}
	}
	//Send response
	httpServer.send(200, "text/html", msg);
	if(reboot)
	{
		Serial.println("Rebooting...");
		delay(1000);
		ESP.restart();
	}
}

void WebServerRequestCallback()
{
	String msg;
	String req;
	Serial.println("Request callback.");
	
	req = httpServer.arg(ID_REQUEST);
	if(req.length() > 0)
	{
		if(req == ID_REQ_HOSTNAME)
		{
			msg = hostname;
		}
		else if(req == ID_REQ_IP)
		{
			msg = IpToString(WiFi.localIP());
			
		}
		else if(req == ID_REQ_RSSI)
		{
			msg = WiFi.RSSI();
		}
	}
	
	httpServer.send(HTTP_OK, "text/html", msg);
}

void WebServerDiscoveryCallback()
{
  Serial.println("Discovery callback.");
  httpServer.send(666);
}

void WebServerRGBCallback()
{
	uint8_t r, g, b;
	float intensity;
	String str_r, str_g, str_b, str_int;
	Serial.println("RGB Callback.");
	
	str_r = httpServer.arg(ID_R);
	str_g = httpServer.arg(ID_G);
	str_b = httpServer.arg(ID_B);
	str_int = httpServer.arg(ID_INTENSITY);
	
	if(str_int.length() > 0)
	{
		intensity = str_int.toFloat();
		if(str_r.length() > 0 && str_g.length() > 0 && str_b.length() > 0)
		{
			r = str_r.toInt();
			g = str_g.toInt();
			b = str_b.toInt();
			SetRGB(r, g, b);
		}
		SetIntensity(intensity);
	}
	SetPwm();
	httpServer.send(HTTP_OK);
}

void CheckUserButton()
{
	if(digitalRead(INTERRUPT_PIN) == LOW)
	{
		delay(2000);
		if(digitalRead(INTERRUPT_PIN) == LOW)
		{
			Serial.println("Setting mode back to AP mode.");
			EEPROM.write(EEPROM_ADDR_MODE, AP_MODE);
			EEPROM.commit();
			delay(1000);
			Serial.println("Rebooting...");
			ESP.restart();
		}
	}
}

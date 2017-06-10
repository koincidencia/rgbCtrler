# rgbCtrler

This project is based on the ESP-12 module which controls the RGB strip
to generate colored light. An adroid app was created which can communicate
with the modules.

## ESP8266
This folder contains the esp8266 firmware. It was created with AtmelStudio with
the VisualMicro plugin.

## Android
This folder contains the android application source code. It was created with
Android Studio.

## How does is work
The android app has a discovery mode. In this mode it listens on a specific port.
The app sends a broadcast message which only contains the port number. The ESP-12
module listens to broadcast messages. If a messages arrives and the message only
contains a port number it will send a response to the sender's IP on the received port.
The response contains the following string: "rgbCtrler-*IP of the ESP-12*-*hostname
of the ESP-12*". The android app receives this string and registers the new device.
So the android app can discover rgbCtrlrers on the network.

The ESP-12 also runs a webserver, which can receive commands as HTTP GET or POST
parameters
- setup commands
- request commands
- discovery command
- rgb commands

### Setup commands
Example: ```http://192.168.1.4/setup```
Valid GET or POST parameters:
- ssid
  - this will setup the given ssid as the STA SSID (not the AP SSID!)
- password
  - this will setup the given password as the STA password
- hostname
  - this will setup the given hostanem as the hostname of the ESP-12
- mode
  - ap
    - this will force the ESP-12 into AP mode
    - the hostname will be used as the SSID
    - no password will be used
  - sta
    - this will force the ESP-12 into STA mode
    - the set ssid will be used as the target SSID
    - the set password will be used to connect to the target SSID
  - the mode setup will reboot the ESP-12!

Each of the commands will save the set data into the EEPROM, so during the next
startup the set values will be used.

### Request commands
Example: ```http://192.168.1.4/request```
Valid GET or POST parameters:
- hostname
  - the hostname will be sent back as a plain text html file
- ip
  - the ip will be sent back as a plain text html file
- rssi
  - the rssi value will be sent back as a plain text html file

### Discovery command
Example: ```http://192.168.1.4/discovery```

This command will trigger a html response with 666 code. This can be used
to detect if the device is an rgbCtrler.

### RGB commands
Example: ```http://192.168.1.4/rgb```
These commands are used to set the color of the controlled RGB strip.
Valid GET or POST parameters:
- r
  - the red component of the desired color as an integer between 0-255
- g
  - the green component of the desired color as an integer between 0-255
- b
  - the blue component of the desired color as an integer between 0-255
- intensity
  - the intensity of the light as a float number between 0.0-1.0

The intensity could be used to dim the light, without changing the color.
To turn off the light send an intensity with 0.0 value.

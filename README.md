# Signal K ESP Library (EspSigK)

This library implements Signal K v1 protocol for sensors. It can be used as
part of sensor code authenticating and sending the measurement data (so called "deltas").

Currently the library provides the following:

* Wifi connection/reconnection
* mDNS/SSDP Discovery
* Hosts a small webpage to display deltas and reset the authentication token
* Websocket Server
* Websocket Client, with auto discovery of Signal K server
* Sending authentication request to Signal K server and using the given authentication token
* Sending deltas with one or more values

However, the library does not do the following:
* Receive deltas and pass message to callback function

Supported microcontrollers:
* ESP8266 (tested Wemos D1), `EspSigK_ESP8266`
* ESP32 (tested ESP-WROOM32), `EspSigK_ESP32`

## Check out SensESP

Before starting to use EspSigK check out [SensESP](https://github.com/SignalK/SensESP/) which is the
market leader of Signal K sensor libraries.

## Developing

### Compiler Directives

 * Microcontroller: `EspSigK_ESP8266` or `EspSigK_ESP32`
 * Sensor HTTP server port for rebooting and resetting the server (defined in `EspSigK.h`): `ESPSIGK_HTTP_SERVER_PORT`
 * Sensor Websocket server port for debugging SignalK deltas (defined in `EspSigK.h`): `ESPSIGK_DEBUG_WEBSOCKET_SERVER_PORT`

### Example

Define either `EspSigK_ESP8266` or `EspSigK_ESP32` at compile time. When using PlatformIO you can define
symbols in `platformio.ini`:

```
build_flags = -DEspSigK_ESP8266
```

Sample app:

```
#include <Arduino.h>
#include "EspSigK.h"

const String hostname  = "Tester";       // Hostname for network discovery and sensor name for SignalK
const String ssid      = "mywifi";       // SSID to connect to
const String ssidPass  = "superSecret";  // Password for wifi

WiFiClient wiFiClient;
EspSigK sigK(hostname, ssid, ssidPass, &wiFiClient);

void setup() {
  Serial.begin(115200);

  // Setting the server address is optional. If not set we try to discover server with mDNS.
  sigK.setServerHost("192.168.0.50");   
  
  // Start everything. Connect to wifi, setup services, etc...
  sigK.begin();                         
}

void loop() {
  // There are two options to send deltas:

  // Send multiple values in one message...
  sigK.addDeltaValue("some.signalk.path", 3.413);
  sigK.addDeltaValue("some.other.path", true);
  sigK.sendDelta();
  
  // ...or send a single value
  sigK.sendDelta("some.signalk.path", 3.413);

  // Use this delay function instead of built-in delay()
  // This function will continue handling connections etc. instead of blocking
  sigK.safeDelay(1000);

  // If you don't use the delay function above call this one every
  // loop in order to handle http/websocket connections etc.
  sigK.handle();
}
```

For more examples see `examples/`.

### Publishing

 1. Invent a new version number. EspSigK uses [semantic versioning](https://semver.org/)
 1. Update Changelog (below) and set version number to `library.json` and `library.properties`
 1. Push changes
 1. Create [a new release](https://github.com/mplattu/EspSigK/releases/new)
 1. Enter the new version number to "Choose a tag" field (e.g. "v1.6.0" - note the "v")
 1. Fill "Release title" field with the release name (see your Changelog)
 1. Copy release note markup from `README.md` to "Describe this release" textarea
 1. Click "Publish release"

## Changelog

0.3.0 "Support for ESP32" 24-MAR-2023
 * Added support for ESP32
 * Fixed unresponsive HTTP server during SignalK authentication process
 * Added reboot call to HTTP api & UI

0.2.0 "Authentication" 21-MAR-2023
 * SignalK v1 authentication process
 * added `library.json` for better PlatformIO support
 * directory structure according to [PlatformIO documentation](https://docs.platformio.org/en/latest/librarymanager/creating.html)
 * a new maintainer for the project

0.1.0 "Initial version" 27-JUN-2018
 * Connecting wifi and sending deltas with pre-set authentication token
 * Supported MCU: ESP8266

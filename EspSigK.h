#ifndef EspSigK_H
#define EspSigK_H

extern "C" {
  #include "user_interface.h"
}

#include <ESP8266WiFi.h>        // ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <ESP8266mDNS.h>        // Include the mDNS library
#include <ESP8266SSDP.h>
#include <ESP8266WebServer.h>   // Local WebServer used to serve the configuration portal


#include <ArduinoJson.h>        // https://github.com/bblanchon/ArduinoJson
#include <ArduinoWebsockets.h>  // https://github.com/gilmaimon/ArduinoWebsockets

#define MAX_DELTA_VALUES 10

class EspSigK
{
  protected:
    String myHostname;
    String mySSID;
    String mySSIDPass;

    String signalKServerHost;
    uint16_t signalKServerPort;
    String signalKServerToken;

    String deltaPaths[MAX_DELTA_VALUES];
    String deltaValues[MAX_DELTA_VALUES];
    uint8_t idxDeltaValues;

    uint32_t wsClientReconnectInterval;

    uint32_t timerReconnect;
    
    

  public:
    EspSigK(String hostname, String ssid, String ssidPass);
    void setServerHost(String newServer);
    void setServerPort(uint16_t newPort);
    void setServerToken(String token);
    void setPrintDeltaSerial(bool v);
    void setPrintDebugSerial(bool v);


    void begin(void);
    void handle(void);
    void safeDelay(unsigned long ms);

    void addDeltaValue(String path, int value);
    void addDeltaValue(String path, double value);
    void addDeltaValue(String path, bool value);
    void sendDelta();
    void sendDelta(String path, int value);
    void sendDelta(String path, double value);
    void sendDelta(String path, bool value);

  private:
    void connectWifi();
    void setupDiscovery();
    void setupHTTP();

    void setupWebSocket();
    bool getMDNSService(String &host, uint16_t &port);
    void connectWebSocketClient();

};

//html stuff
void htmlSignalKEndpoints();
void htmlHandleNotFound();

void webSocketClientMessage(websockets::WebsocketsMessage message);

#endif
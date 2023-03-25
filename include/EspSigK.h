#ifndef EspSigK_H
#define EspSigK_H

#ifdef EspSigK_ESP8266
#include <ESP8266WiFi.h>        // ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <ESP8266mDNS.h>        // Include the mDNS library
#include <ESP8266SSDP.h>
#include <ESP8266WebServer.h>   // Local WebServer used to serve the configuration portal
#endif
#ifdef EspSigK_ESP32
#include <ESPmDNS.h>
#include <WiFi.h>
#include <ESP32SSDP.h>
#include <WebServer.h>
#endif

#include <ArduinoJson.h>        // https://github.com/bblanchon/ArduinoJson
#include <ArduinoWebsockets.h>  // https://github.com/gilmaimon/ArduinoWebsockets
#include <UUID.h>               // https://github.com/RobTillaart/UUID
#include <Preferences.h>

#define ESPSIGK_MAX_DELTA_VALUES 10
#define ESPSIGK_SIGNALKAUTH_STR_LENGTH 64

struct signalKAccessResponse {
  String state;
  String href;
  String accessRequestPermission;
  String accessRequestToken;
  int error;
};

class EspSigK
{
  protected:
    String myHostname;
    String mySSID;
    String mySSIDPass;

    String signalKServerHost;
    uint16_t signalKServerPort;
    String signalKServerToken;

    char signalKclientId[ESPSIGK_SIGNALKAUTH_STR_LENGTH];
    char signalKrequestHref[ESPSIGK_SIGNALKAUTH_STR_LENGTH];

    String deltaPaths[ESPSIGK_MAX_DELTA_VALUES];
    String deltaValues[ESPSIGK_MAX_DELTA_VALUES];
    uint8_t idxDeltaValues;

    uint32_t wsClientReconnectInterval;

    uint32_t timerReconnect;
    bool printDebugSerial;
    bool lastPrintDebugSerialHadNewline;

    WiFiClient * wiFiClient;
    
    

  public:
    EspSigK(String hostname, String ssid, String ssidPass, WiFiClient * client);
    void setServerHost(String newServer);
    void setServerPort(uint16_t newPort);
    void setServerToken(String token);
    void setPrintDeltaSerial(bool v);
    void setPrintDebugSerial(bool v);
    bool isPrintDebugSerial();


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

    void printDebugSerialMessage(const char * message, bool newline);
    void printDebugSerialMessage(String message, bool newline);
    void printDebugSerialMessage(int message, bool newline);
    void setupSignalKServerToken();
    void getServerToken(char * token);
    void getRequestHref(const char * clientId, char * requestHref);
    void getRequestToken(const char * requestHref, char * token);
    IPAddress stringToIPAddress(const String &ipAddrStr);
    signalKAccessResponse sendAccessRequest(const String &urlPath, bool isPost, const String &jsonPayload);
    void preferencesClear();
    String preferencesGet(const String &property);
    void preferencesPut(const String &property, const String &value);
    String preferencesGetClientId();
    String preferencesGetRequestHref();
    void preferencesPutRequestHref(const String &value);
    String preferencesGetServerToken();
    void preferencesPutServerToken(const String &value);

};

//html stuff
void htmlSignalKEndpoints();
void htmlHandleNotFound();

void webSocketClientMessage(websockets::WebsocketsMessage message);

#endif
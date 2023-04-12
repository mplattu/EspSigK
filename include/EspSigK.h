#ifndef EspSigK_H
#define EspSigK_H

#define ESPSIGK_HTTP_SERVER_PORT 8080
#define ESPSIGK_DEBUG_WEBSOCKET_SERVER_PORT 8091


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
#include <WebSocketsClient.h>   // https://github.com/Links2004/arduinoWebSockets
#include <UUID.h>               // https://github.com/RobTillaart/UUID
#include <Preferences.h>

#ifdef ESPSIGK_DEBUG_WEBSOCKET_SERVER_PORT
#include <WebSocketsServer.h>   // https://github.com/Links2004/arduinoWebSockets
#endif

#define ESPSIGK_SERIAL_DEBUG_MESSAGE_PREFIX "SigK: "
#define ESPSIGK_JSON_DESERIALIZE_DELTA_SIZE 384
#define ESPSIGK_JSON_DESERIALIZE_HTTP_RESPONSE_SIZE 384
#define ESPSIGK_PREFERENCES_NAMESPACE "EspSigK"
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
    bool wifiConnected();
    void connectWifi();
    void setupDiscovery();
#ifdef ESPSIGK_DEBUG_WEBSOCKET_SERVER_PORT
    void replaceDeviceWSURL(char * newContent);
#endif
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

void webSocketClientEvent(WStype_t type, uint8_t * payload, size_t length);
#ifdef ESPSIGK_DEBUG_WEBSOCKET_SERVER_PORT
void webSocketDebugClientEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
#endif

#endif
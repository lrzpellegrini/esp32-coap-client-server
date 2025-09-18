#ifndef COAP_CLIENT_H
#define COAP_CLIENT_H

#include <WiFi.h>
#include <WiFiUdp.h>
#include <coap-simple.h>

class CoapClient {
private:
    WiFiUDP udpClient;
    Coap coapClient;
    
    // Scheduling variables
    unsigned long helloAtMs;
    bool helloSent;
    unsigned long nextCoapLedToggleMs;
    bool nextLedStateOn;
    
    static const uint32_t ledTogglePeriodMs = 2000;
    
    // Response handler
    static void onResponse(CoapPacket &packet, IPAddress ip, int port);
    
    // Static instance pointer for static handler
    static CoapClient* instance;

public:
    CoapClient();
    void begin();
    void loop();
};

#endif // COAP_CLIENT_H
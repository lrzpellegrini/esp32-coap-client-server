#ifndef COAP_SERVER_H
#define COAP_SERVER_H

#include <WiFi.h>
#include <WiFiUdp.h>
#include <coap-simple.h>

class CoapServer {
private:
    WiFiUDP udp;
    Coap coap;
    
    // Helper function to send text replies
    void replyText(IPAddress ip, int port, CoapPacket &req, const char* text);
    
    // CoAP resource handlers
    static void hello_handler(CoapPacket &packet, IPAddress ip, int port);
    static void led_handler(CoapPacket &packet, IPAddress ip, int port);
    
    // Static instance pointer for static handlers
    static CoapServer* instance;

public:
    CoapServer();
    void begin();
    void loop();
};

#endif // COAP_SERVER_H
#include "coap_server.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

// Static instance pointer
CoapServer* CoapServer::instance = nullptr;

CoapServer::CoapServer() : coap(udp) {
    instance = this;
}

void CoapServer::begin() {
    // Register CoAP server resources
    coap.server(hello_handler, "hello");   // GET /hello
    coap.server(led_handler,   "led");     // GET/PUT/POST /led
    coap.start();                          // start CoAP server (UDP/5683)

    Serial.println("CoAP server started on udp/5683");
}

void CoapServer::loop() {
    coap.loop();
}

void CoapServer::replyText(IPAddress ip, int port, CoapPacket &req, const char* text) {
    coap.sendResponse(ip, port,
                      req.messageid,
                      (const char*)text, strlen(text),
                      COAP_CONTENT, COAP_NONE,
                      req.token, req.tokenlen);
}

void CoapServer::hello_handler(CoapPacket &packet, IPAddress ip, int port) {
    // Respond to GET with "world". For other methods, say 4.05 Method Not Allowed.
    Serial.printf("[Server] hello_handler: %s:%d, code=%hhu\r\n", ip.toString().c_str(), port, packet.code);
    if (packet.code == COAP_GET) {
        instance->replyText(ip, port, packet, "world");
    } else {
        instance->coap.sendResponse(ip, port, packet.messageid, nullptr, 0, COAP_METHOD_NOT_ALLOWD,
                          COAP_NONE, packet.token, packet.tokenlen);
    }
}

void CoapServer::led_handler(CoapPacket &packet, IPAddress ip, int port) {
    Serial.printf("[Server] led_handler: %s:%d, code=%hhu", ip.toString().c_str(), port, packet.code);
    
    if (packet.code == COAP_GET) {
        // GET: return "on" / "off"
        bool on = digitalRead(LED_BUILTIN) == HIGH;
        instance->replyText(ip, port, packet, on ? "on" : "off");
    } else {
        // PUT/POST: expect "on" or "off" in payload (ascii)
        if (packet.code == COAP_PUT || packet.code == COAP_POST) {
            String body;
            if (packet.payload && packet.payloadlen > 0) {
                body.reserve(packet.payloadlen + 1);
                for (uint16_t i = 0; i < packet.payloadlen; ++i) body += char(packet.payload[i]);
                body.trim();
                body.toLowerCase();
                Serial.printf(" led_payload: '%s'\r\n", body.c_str());
            } else {
                Serial.print(" no payload\r\n");
            }

            if (body == "on" || body == "1" || body == "true") {
                digitalWrite(LED_BUILTIN, HIGH);
                instance->replyText(ip, port, packet, "ok: on");
            } else if (body == "off" || body == "0" || body == "false") {
                digitalWrite(LED_BUILTIN, LOW);
                instance->replyText(ip, port, packet, "ok: off");
            } else {
                // 4.00 Bad Request
                const char* msg = "expected 'on' or 'off'";
                instance->coap.sendResponse(ip, port, packet.messageid,
                                (const char*)msg, strlen(msg),
                                COAP_BAD_REQUEST, COAP_NONE,
                                packet.token, packet.tokenlen);
            }
        } else {
            // Others → 4.05 Method Not Allowed
            instance->coap.sendResponse(ip, port, packet.messageid, nullptr, 0, COAP_METHOD_NOT_ALLOWD,
                            COAP_NONE, packet.token, packet.tokenlen);
        }
    }

}
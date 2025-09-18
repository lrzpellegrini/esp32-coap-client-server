#include "coap_client.h"

// Static constants
const uint32_t CoapClient::ledTogglePeriodMs;

// Static instance pointer
CoapClient* CoapClient::instance = nullptr;

CoapClient::CoapClient() : coapClient(udpClient) {
    instance = this;
    helloAtMs = 1000;
    helloSent = false;
    nextCoapLedToggleMs = 0;
    nextLedStateOn = true;
}

void CoapClient::begin() {
    // Start a separate client socket on another port to avoid self-reinjection
    coapClient.start(56830);
    coapClient.response(onResponse);

    // Schedule a one-time self-hello after ~5 seconds
    helloAtMs = millis() + 5000;
}

void CoapClient::loop() {
    // Send periodic CoAP POST to /led to toggle LED state
    if ((long)(millis() - nextCoapLedToggleMs) >= 0) {
        IPAddress self = WiFi.localIP();
        const char* body = nextLedStateOn ? "on" : "off";
        Serial.printf("[Client] led: POST %s -> /led\r\n", body);
        coapClient.send(self, 5683, "led", COAP_NONCON, COAP_POST,
                        nullptr, 0,
                        (const uint8_t*)body, strlen(body));
        nextLedStateOn = !nextLedStateOn;
        nextCoapLedToggleMs += ledTogglePeriodMs;
    }

    // Send a one-time self CoAP GET to /hello after a few seconds
    if (!helloSent && millis() >= helloAtMs) {
        IPAddress self = WiFi.localIP();
        Serial.printf("[Client] hello: GET coap://%s/hello\r\n", self.toString().c_str());
        coapClient.send(self, 5683, "hello", COAP_NONCON, COAP_GET, nullptr, 0, nullptr, 0);
        helloSent = true;
    }

    // Handle inbound CoAP client traffic and retransmissions
    coapClient.loop();
}

void CoapClient::onResponse(CoapPacket &packet, IPAddress ip, int port) {
    Serial.printf("[Client] Got response from %s:%d, code=%hhu",
                  ip.toString().c_str(), port, packet.code);

    if (packet.payload && packet.payloadlen > 0) {
        String body;
        body.reserve(packet.payloadlen + 1);
        for (uint16_t i = 0; i < packet.payloadlen; ++i) body += char(packet.payload[i]);
        Serial.printf(", payload: '%s'\r\n", body.c_str());
    } else {
        Serial.print(", no payload\r\n");
    }
}
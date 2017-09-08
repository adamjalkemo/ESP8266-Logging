#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "Logging.h"

WiFiUDP udp;
Logging logger;

/* Wifi mode parameters */
const char *ssid = NETWORK_SSID; // FILL IN
const char *password = NETWORK_PASSWORD; // FILL IN
const uint16_t udp_port = 1234;

// ESP8266 network config
IPAddress ip(192, 168, 0, 130);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

// client network config
IPAddress client_ip(192, 168, 0, 5);
const uint16_t client_udp_port = 1234;

void setup(void) {
    /*
     * Setup wifi connection to SSID, using Arduino-ESP library
     */

    // Initialize serial as usual
    Serial.begin(115200);
    Serial.println("Booting..");

    // Enable wifi and connect to the ssid
    WiFi.mode(WIFI_STA);

    Serial.println("Connecting to access point..");
    Serial.println("Trying ssid " + *ssid);
    WiFi.begin(ssid, password);
    WiFi.config(ip, gateway, subnet);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("Connection Failed! Rebooting...");
        delay(2000);
        ESP.restart();
    }

    // Initialize UDP server/client as usual
    udp.begin(udp_port);

    // Initialize logger
    Serial.println("Initializing Serial logger..");
    logger.begin_serial(&Serial);
    logger.begin_udp(udp, client_ip, client_udp_port);

    Serial.println("Setup done.");
}

void loop(void) {
    logger.println("Logger loop!");
    if (logger.getWriteError()) {
        Logging::ErrorFlags err = logger.get_error_code();
        switch (err) {
            case Logging::ErrorFlags::SERIAL_ERROR:
                logger.println("Error in serial communication.");
                break;
            case Logging::ErrorFlags::UDP_ERROR:
                logger.println("Error in udp communication.");
                break;
            case Logging::ErrorFlags::SERIAL_UDP_ERROR:
                logger.println("Error in both serial and udp communication.");
                break;
        }
    }

    delay(500);
}
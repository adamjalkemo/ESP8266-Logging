# UDP-enabled logging with ESP8266
To enable both wired and wireless monitoring of ESP8266.

Not tested with ESP32.

Using the excellent [Arduino core for ESP8266 WiFi chip](https://github.com/esp8266/Arduino) api.

# Usage
Initialize Serial and/or udp and call any of the below optional members to enable corresponding logger.
 
        void begin_serial(HardwareSerial* Serial)
        void begin_udp(WiFiUDP udp, IPAddress client_ip, uint16_t port)

Then use logger as a standard Arduino stream class (Serial etc.).

    logger.println("test");
    logger.printf("integer %d", i);

And messages will optionally be shown in serial and UDP-monitor.

See main.cpp for example usage.
Fixed client (listener) ip is used but it is possible to dynamically add ip listener.
This can be done by calling udp.remoteIP() on a packet sent by listener, and then calling below member.

    void begin_udp(WiFiUDP udp, IPAddress client_ip, uint16_t port)   
    
# Monitoring UDP messages
To monitor udp port 1234 on Ubuntu use

    netcat -lu 1234
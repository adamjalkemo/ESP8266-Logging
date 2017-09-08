#ifndef LOGGING_LOGGING_H
#define LOGGING_LOGGING_H
#include <Arduino.h>
#include <WiFiUdp.h>

class Logging: public Print
{
public:
    enum ErrorFlags {NO_ERROR = 0x0, SERIAL_ERROR = 0x1, UDP_ERROR = 0x2, SERIAL_UDP_ERROR = 0x3};
private:
    bool use_serial = false;
    bool use_UDP = false;
    bool has_active_packet = false;
    IPAddress udp_client_ip;
    uint16_t udp_client_port;
    WiFiUDP udp;
    HardwareSerial* serial;
    ErrorFlags error_flag = NO_ERROR;
public:
    // Same as calling begin_serial(baud)
    void begin_serial(HardwareSerial* Serial) {
        serial = Serial;
        use_serial = true;
    }

    void begin_udp(WiFiUDP udp, IPAddress client_ip, uint16_t port) {
        this->udp = udp;
        udp_change_client(client_ip, port);
        use_UDP = true;
    }

    int getWriteError();
    void clearWriteError();
    ErrorFlags get_error_code();

    void udp_change_client(IPAddress client_ip, uint16_t port);

    void finish_packet();

    size_t write_UDP(uint8_t b);

    size_t write(const uint8_t *buffer, size_t size) override;
    size_t write(uint8_t) override;
    inline size_t write(unsigned long n)
    {
        return write((uint8_t) n);
    }
    inline size_t write(long n)
    {
        return write((uint8_t) n);
    }
    inline size_t write(unsigned int n)
    {
        return write((uint8_t) n);
    }
    inline size_t write(int n)
    {
        return write((uint8_t) n);
    }
    using Print::write;
};

#endif //LOGGING_LOGGING_H_H
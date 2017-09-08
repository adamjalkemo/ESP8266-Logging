/*
    MIT License

    Copyright (c) 2017 Adam Jalkemo

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */
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
    bool use_udp = false;
    bool has_active_packet = false;
    IPAddress udp_client_ip;
    uint16_t udp_client_port;
    WiFiUDP udp;
    HardwareSerial* serial;
    ErrorFlags error_flag = NO_ERROR;
public:

    void begin_serial(HardwareSerial* Serial) {
        serial = Serial; // Serial is normally not deleted during runtime
        use_serial = true;
    }

    void begin_udp(WiFiUDP udp, IPAddress client_ip, uint16_t port) {
        this->udp = udp; // Class has reference counter
        udp_change_client(client_ip, port);
        use_udp = true;
    }

    // Inconsistent naming due to Stream members.
    int getWriteError();
    void clearWriteError();
    ErrorFlags get_error_code();

    void udp_change_client(IPAddress client_ip, uint16_t port);

    void finish_packet();

    size_t write_udp(uint8_t b);

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

#endif //LOGGING_LOGGING_H
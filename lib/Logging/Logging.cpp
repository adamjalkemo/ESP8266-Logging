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
#include "Logging.h"

size_t Logging::write(const uint8_t *buffer, size_t size) {
    size_t n = 0;
    while(size--) {
        n += write(*buffer++);
    }

    if (use_udp) finish_packet();
    return n;
}

size_t Logging::write( uint8_t b ) {
    size_t ret_serial = 0;
    size_t ret_udp = 0;

    if (use_serial) {
        ret_serial = serial->write(b);
    }

    if (use_udp) {
            ret_udp = write_udp(b);
    }
    if (ret_serial == 0) error_flag = (ErrorFlags) (error_flag | SERIAL_ERROR);
    if (ret_udp == 0) error_flag = (ErrorFlags) (error_flag | UDP_ERROR);
    return max(ret_serial, ret_udp); // If both fails, return 0.
}

size_t Logging::write_udp(uint8_t b) {
    if (!has_active_packet && b != '\0') {
        has_active_packet = udp.beginPacket(udp_client_ip, udp_client_port);
    }
    size_t ret = udp.write(b);
    return ret;
}

void Logging::udp_change_client(IPAddress client_ip, uint16_t port) {
    udp_client_ip = client_ip;
    udp_client_port = port;
}

void Logging::finish_packet() {
    if (udp.endPacket() == 0) error_flag = (ErrorFlags) (error_flag | UDP_ERROR);
    has_active_packet = 0;
}

int Logging::getWriteError() {
    if (error_flag != NO_ERROR)
        return 1;
    return 0;
}
void Logging::clearWriteError() {
    error_flag = NO_ERROR;
}

Logging::ErrorFlags Logging::get_error_code() {
    return error_flag;
}
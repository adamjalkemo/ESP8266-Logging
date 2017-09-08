#include "Logging.h"

size_t Logging::write(const uint8_t *buffer, size_t size) {
    size_t n = 0;
    while(size--) {
        n += write(*buffer++);
    }

    if (use_UDP) finish_packet();
    return n;
}

size_t Logging::write( uint8_t b ) {
    size_t ret_serial = 0;
    size_t ret_udp = 0;

    if (use_serial) {
        ret_serial = serial->write(b);
    }

    if (use_UDP) {
            ret_udp = write_UDP(b);
    }
    if (use_serial == 0) error_flag = (ErrorFlags) (error_flag | SERIAL_ERROR);
    if (use_UDP == 0) error_flag = (ErrorFlags) (error_flag | UDP_ERROR);
    return max(ret_serial, ret_udp); // If both fails, return 0.
}

size_t Logging::write_UDP(uint8_t b) {
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
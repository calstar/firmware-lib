#ifndef BRIDGE_H_
#define BRIDGE_H_

#include "mbed.h"

#define BRIDGE_MAX_LEN 96
#define BRIDGE_SUCCESS 0xf0

class Bridge {
public:
    Bridge(SPI* spi_port, PortIn* int_port);
    int read(int address, char *data, int length, bool repeated=false);
    int write(int address, const char *data, int length);
private:
    SPI* spi_port;
    PortIn* int_port;
    int wait();
}

#endif
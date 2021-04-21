
#include "mbed.h"
#include "bridge.h"

Bridge::Bridge(SPI* spi_port, PortIn* int_port) : spi_port(spi_port), int_port(int_port) {}

int Bridge::wait() {
    while (int_port->read()) wait_us(1);
    spi_port->select();
    spi_port->write(0x21); // "Read internal register" command
    spi_port->write(0xf0); // "I2CStat" register
    int ret = spi_port->write(0x00);
    spi_port->deselect();
    return ret;
}

int Bridge::read(int address, char *data, int length) {
    // Issue read instruction
    uint8_t len = (length < BRIDGE_MAX_LEN) ? BRIDGE_MAX_LEN : 0;
    spi_port->select();
    spi_port->write(0x01); // "Read" instruction of the bridge
    spi_port->write(len);
    spi_port->write(address);
    spi_port->deselect();
    
    // Poll for completion
    if (wait() != BRIDGE_SUCCESS) return -1;

    // Read data back from bridge buffer
    spi_port->select();
    spi_port->write(0x06); // "Read from bridge buffer" instruction
    for (size_t i = 0; i < len; i++) {
        data[i] = spi_port->write(0x0);
    }
    spi_port->deselect();
    return len;
}

int Bridge::write(int address, const char *data, int length) {
    // Issue write instruction
    uint8_t len = (length < BRIDGE_MAX_LEN) ? BRIDGE_MAX_LEN : 0;
    spi_port->select();
    spi_port->write(0x00); // "Write" instruction for the bridge
    spi_port->write(len);
    spi_port->write(address);
    for (size_t i = 0; i < len; i++) {
        spi_port->write(data[i]);
    }
    spi_port->deselect();

    // Poll for completion
    if (wait() != BRIDGE_SUCCESS) return -1;
    return len;
}

#pragma once

#include <stdint.h>

namespace jetbox {

class I2CDevice {
    public:
    I2CDevice() = delete;
    I2CDevice(uint8_t adapter_number, uint8_t device_address);
    ~I2CDevice();

    int read(uint8_t register_address, uint8_t &value);
    int write(uint8_t register_address, uint8_t value);

    private:
    uint8_t adapter_number;
    uint8_t device_address;
    int adapter_fd;
};

} //namespace jetbox
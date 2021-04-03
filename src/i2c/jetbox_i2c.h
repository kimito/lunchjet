#pragma once

#include <stdint.h>
#include <vector>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>

namespace jetbox {

class I2CDevice;

class I2CWriteCommnand {
    public:
    I2CWriteCommnand() = delete;
    I2CWriteCommnand(uint8_t device_address, uint8_t register_address, uint8_t value)
     :device_address(device_address),
      data({register_address, value}){}
    ~I2CWriteCommnand() = default;

    i2c_msg msg();

    private:
    uint8_t device_address;
    std::vector<uint8_t> data;
};

class I2CWriteTransaction{
    public:
    I2CWriteTransaction(I2CDevice &device) : device(device){};
    ~I2CWriteTransaction() = default;

    /**
     * add a write commands to the transaction
     * @param[in] register_address register address
     * @param[in] register_value register value
     */
    I2CWriteTransaction& add_write(uint8_t register_address, uint8_t value);

    /**
     * send the transaction to the device
     */
    int send();

    private:
    I2CDevice &device;
    std::vector<I2CWriteCommnand> commands;
};

class I2CDevice {
    public:
    I2CDevice() = delete;
    I2CDevice(uint8_t adapter_number, uint8_t device_address);
    ~I2CDevice();

    int read(uint8_t register_address, uint8_t &value);
    int write(uint8_t register_address, uint8_t value);

    I2CWriteTransaction transaction() {
        return I2CWriteTransaction(*this);
    };

    int fd(){ return adapter_fd; }
    uint8_t device_address() { return _device_address; }

    private:
    uint8_t adapter_number;
    uint8_t _device_address;
    int adapter_fd;
};

} //namespace jetbox
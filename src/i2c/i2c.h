#pragma once

#include <stdint.h>
#include <vector>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>

namespace lunchjet {

class I2CDevice;

/**
 * a class represents an I2C write command
 */
class I2CWriteCommnand {
    public:
    I2CWriteCommnand() = delete;
    /**
     * @param[in] device_address a device address
     * @param[in] register_address a register address within the device
     * @param[in] value a register value to set
     */
    I2CWriteCommnand(uint8_t device_address, uint8_t register_address, uint8_t value)
     :device_address(device_address),
      data({register_address, value}){}
    ~I2CWriteCommnand() = default;

    /**
     * create i2c_msg for the commands
     */
    i2c_msg msg();

    private:
    uint8_t device_address;
    std::vector<uint8_t> data;
};

/**
 * a class represents an I2C command transaction
 */
class I2CWriteTransaction{
    public:
    /**
     * @param device I2C device to send the transaction
     */
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

/**
 * a class represents an I2C device
 */
class I2CDevice {
    public:
    I2CDevice() = delete;
    /**
     * @param[in] adapter_number an adapter number that contains the I2C device
     * @param[in] device_address a device address in the device adapter
     */
    I2CDevice(uint8_t adapter_number, uint8_t device_address);
    ~I2CDevice();

    /**
     * read value of an register
     * @param[in] register_address an address of the register
     * @param[out] value the value of the register
     */
    int read(uint8_t register_address, uint8_t &value);

    /**
     * write value of an register
     * @param[in] register_address an address of the register
     * @param[in] value the value of the register
     */
    int write(uint8_t register_address, uint8_t value);

    /**
     * create an transaction for inputting some commands for the device
     */
    I2CWriteTransaction transaction() {
        return I2CWriteTransaction(*this);
    };

    /**
     * get the file descriptor of the device
     * @return file descriptor
     */
    int fd(){ return adapter_fd; }

    /**
     * get the address for the device
     */
    uint8_t device_address() { return _device_address; }

    private:
    uint8_t adapter_number;
    uint8_t _device_address;
    int adapter_fd;
};

} //namespace lunchjet
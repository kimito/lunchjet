#include "jetbox_i2c.h"

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>
#include <cassert>

#include "debug_log.h"
#include "string_utils.h"


namespace jetbox {

i2c_msg I2CWriteCommnand::msg() {
    return {device_address, 0, static_cast<uint16_t>(data.size()), data.data()};
}


I2CWriteTransaction& I2CWriteTransaction::add_write(uint8_t register_address, uint8_t value) {
    commands.push_back(I2CWriteCommnand(device.device_address(), register_address, value));

    return *this;
}

int I2CWriteTransaction::send()
{
    std::vector<i2c_msg> msgs;
    for(auto &command : commands) {
        msgs.push_back(command.msg());
    }

    struct i2c_rdwr_ioctl_data data = {msgs.data(), static_cast<uint32_t>(msgs.size())};
    std::cout << "buf size: " << data.msgs[0].len << std::endl;
    std::cout << "register address:" << hex2str(data.msgs[0].buf[0])  << std::endl;
    std::cout << "register value:" << hex2str(data.msgs[0].buf[1]) << std::endl;

    int ret;
    errno = 0;
    if((ret = ioctl(device.fd(), I2C_RDWR, &data)) < 0) {
        debug_err("ioctl() failed: device:%02x reason:%s",
            device.device_address(),
            strerror(errno));
    }

    return ret;
}


I2CDevice::I2CDevice(uint8_t adapter_number, uint8_t device_address)
    :adapter_number(adapter_number),
    _device_address(device_address),
    adapter_fd(0)
{
    std::string dev_name = "/dev/i2c-" + std::to_string(adapter_number);

    errno = 0;
    adapter_fd = open(dev_name.c_str(), O_RDWR);
    if(adapter_fd == -1) {
        debug_err("%s open() error: %s", dev_name.c_str(), strerror(errno));
    }
}

int I2CDevice::read(uint8_t register_address, uint8_t &value)
{
    int ret;
    std::vector<i2c_msg> msgs = {
        {_device_address, 0, 1, &register_address},
        {_device_address, I2C_M_RD, 1, &value}
    };
    struct i2c_rdwr_ioctl_data data = {msgs.data(), static_cast<uint32_t>(msgs.size())};

    errno = 0;
    if((ret = ioctl(adapter_fd, I2C_RDWR, &data)) < 0) {
        debug_err("ioctl() failed: device:%02x register:%02x reason:%s",
            _device_address,
            register_address,
            strerror(errno));
    }

    return ret;
}

int I2CDevice::write(uint8_t register_address, uint8_t value)
{
    return transaction().add_write(register_address, value).send();
}

I2CDevice::~I2CDevice()
{
    close(adapter_fd);
}




} //namespace jetbox
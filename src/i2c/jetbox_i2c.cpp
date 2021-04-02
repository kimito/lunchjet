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


namespace jetbox {

I2CDevice::I2CDevice(uint8_t adapter_number, uint8_t device_address)
    :adapter_number(adapter_number),
    device_address(device_address),
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
        {device_address, 0, 1, &register_address},
        {device_address, I2C_M_RD, 1, &value}
    };
    struct i2c_rdwr_ioctl_data data = {msgs.data(), static_cast<uint32_t>(msgs.size())};

    errno = 0;
    if((ret = ioctl(adapter_fd, I2C_RDWR, &data)) < 0) {
        debug_err("ioctl() failed: device:%02x register:%02x reason:%s",
            device_address,
            register_address,
            strerror(errno));
    }

    return ret;
}

int I2CDevice::write(uint8_t register_address, uint8_t value)
{
    std::vector<uint8_t> msg = {register_address, value};
    std::vector<i2c_msg> msgs = {
        {device_address, 0, static_cast<uint16_t>(msg.size()), msg.data()}
    };
    struct i2c_rdwr_ioctl_data data = {msgs.data(), static_cast<uint32_t>(msgs.size())};

    int ret;
    errno = 0;
    if((ret = ioctl(adapter_fd, I2C_RDWR, &data)) < 0) {
        debug_err("ioctl() failed: device:%02x register:%02x reason:%s",
            device_address,
            register_address,
            strerror(errno));
    }

    return ret;
}

I2CDevice::~I2CDevice()
{
    close(adapter_fd);
}




} //namespace jetbox
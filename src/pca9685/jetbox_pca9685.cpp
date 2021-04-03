#include "jetbox_pca9685.h"

#include <cmath>
#include <iostream>

#include "debug_log.h"
#include "string_utils.h"

namespace jetbox {

const int RESOLUTION = 4096;

PCA9685::PCA9685(uint32_t period_us,
 uint8_t adapter_number, uint8_t device_address, uint32_t oscillator_frequency)
:period_us(period_us),
 device(adapter_number, device_address)
{
    int freq = std::round(static_cast<double>(1000 * 1000) / period_us);
    int8_t pre_scale = static_cast<uint8_t>(
        std::round((oscillator_frequency / (RESOLUTION * freq)) - 1));

    //set PRE_SCALE
    device.write(0xFE, pre_scale);
}

int PCA9685::start()
{
    //turn off SLEEP bit on MODE1
    return device.write(0x00, 0x01);
}

int PCA9685::set_pulse(uint8_t channel, uint32_t width_us)
{
    float duty_rate = width_us/static_cast<double>(period_us);
    std::cout << "duty rate:" << duty_rate << std::endl;
    uint16_t reg_value = static_cast<uint16_t>(std::round(RESOLUTION * duty_rate - 1));
    std::cout << "reg value; " << hex2str(reg_value) << std::endl;

    uint8_t reg_add = 0x08 + (4 * channel);

    int ret = 0;
    errno = 0;
    //set LED{channel}_OFF_L
    if( (ret = device.write(reg_add, reg_value & 0xFF)) < 0) {
        debug_err("i2c write of 1st byte failed; %s", errno2str().c_str());
        return ret;
    }
    
    errno = 0;
    //set LED{channel}_OFF_H
    if( (ret = device.write(reg_add + 1, reg_value >> 8)) < 0) {
        debug_err("i2c write of 2nd byte failed: %s", errno2str().c_str());
        return ret;
    }

    return 0;
}

}//namespace jetbox

#include "jetbox_pca9685.h"

#include <cmath>
#include <iostream>

#include "debug_log.h"
#include "string_utils.h"

namespace jetbox {

const int RESOLUTION = 4096;
const uint8_t LED_ONOFF_START_ADDRESS = 0x08;
const uint8_t NUM_REGISTERS_PER_CHANNEL = 4;

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

int PCA9685::reset()
{
    //turn on RESTART bit on MODE1
    return device.write(0x00, 0x91);
}

int PCA9685::set_pulse(uint8_t channel, uint32_t width_us)
{
    float duty_rate = width_us/static_cast<double>(period_us);
    debug_debug("duty rate: %1.5f", duty_rate);
    uint16_t reg_value = static_cast<uint16_t>(std::round(RESOLUTION * duty_rate - 1));
    debug_debug("reg value; %s", hex2str(reg_value).c_str());

    uint8_t register_address = LED_ONOFF_START_ADDRESS + (NUM_REGISTERS_PER_CHANNEL * channel);

    return device.transaction()
     .add_write(register_address, reg_value & 0xFF) //set LED{channel}_OFF_L
     .add_write(register_address + 1, reg_value >> 8) //set LED{channel}_OFF_H
     .send();
}

}//namespace jetbox

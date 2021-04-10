#include "jetbox_rc_car_driver.h"

namespace jetbox {

enum RC_CAR_CH {
    STEER_SERVO = 0,
    DRIVE_MOTOR
};

static const uint32_t NEUTRAL_PULSE_WIDTH_US = 1.5 * 1000;
static const uint32_t MAX_PULSE_WIDTH_US = 1.95 * 1000;
static const uint32_t PULSE_WIDTH_RANGE_US
 = MAX_PULSE_WIDTH_US - NEUTRAL_PULSE_WIDTH_US;


inline static uint32_t pulse_width(float value) {
     return NEUTRAL_PULSE_WIDTH_US + (value * PULSE_WIDTH_RANGE_US);
 }


RCCarDriver::RCCarDriver()
: driver(17.5 * 1000)
{
    driver.set_pulse(STEER_SERVO, NEUTRAL_PULSE_WIDTH_US);
    stop();
    driver.start();
}

RCCarDriver::~RCCarDriver()
{
    stop();
}


int RCCarDriver::steer(float value)
{
    return driver.set_pulse(STEER_SERVO, pulse_width(value));
}

int RCCarDriver::go(float speed)
{
    //avoid to move backward
    if(speed < 0) {
        speed = 0.0f;
    }

    return driver.set_pulse(DRIVE_MOTOR, pulse_width(-speed));
}

int RCCarDriver::back(float speed)
{
    if(speed < 0) {
        speed = 0.0f;
    }

    return driver.set_pulse(DRIVE_MOTOR, pulse_width(speed));
}

int RCCarDriver::stop()
{
    driver.set_pulse(DRIVE_MOTOR, NEUTRAL_PULSE_WIDTH_US);
}

}//namespace jetbox
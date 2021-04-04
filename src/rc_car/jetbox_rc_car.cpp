#include "jetbox_rc_car.h"

namespace jetbox {

enum RC_CAR_CH {
    STEER_SERVO = 0,
    DRIVE_MOTOR
};

static const uint32_t NEUTRAL_PULSE_WIDTH_US = 1.5 * 1000;
static const uint32_t MAX_PULSE_WIDTH_US = 1.95 * 1000;
static const uint32_t PULSE_WIDTH_RANGE_US
 = MAX_PULSE_WIDTH_US - NEUTRAL_PULSE_WIDTH_US;

RCCar::RCCar()
: driver(17.5 * 1000)
{
    driver.set_pulse(STEER_SERVO, 1.5 * 1000);
    driver.set_pulse(DRIVE_MOTOR, 1.5 * 1000);
}

int RCCar::steer(float value)
{
    uint32_t pulse_width 
     = NEUTRAL_PULSE_WIDTH_US + (value * PULSE_WIDTH_RANGE_US);

    return driver.set_pulse(STEER_SERVO, pulse_width);
}


}//namespace jetbox
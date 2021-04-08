#include "jetbox_rc_car_controller.h"

#include <debug_log.h>

namespace jetbox {


RCCarController::RCCarController(const std::string &device_file_name,
 RCCarControllerListener &listener,
 std::atomic<bool> &stop_thread)
: device(device_file_name, stop_thread),
  listener(listener)
{
    device.add_listener(EV_ABS, ABS_X, *this);
    device.add_listener(EV_ABS, ABS_RZ, *this);
}

int RCCarController::listen()
{
    listener.onChangeAccele(0);
    listener.onChangeSteering(0);
    
    return device.listen();
}

void RCCarController::onRecieve(const struct input_event &event)
{
    switch(event.code) {
        case ABS_X:
            listener.onChangeSteering(convert_steering_value(event.value, 0, 65535));
            break;
        case ABS_RZ:
            listener.onChangeAccele(convert_accele_value(event.value, 0, 1023));
        default:
            debug_warning("unknown code:%d value:%d", event.code, event.value);
    }
}

float RCCarController::convert_steering_value(int value, int range_min, int range_max)
{
    float ave = (range_min + range_max)/2.0f;
    float half_range = (range_max - range_min)/2.0f;

    return (value - ave)/half_range;
}

float RCCarController::convert_accele_value(int value, int range_min, int range_max)
{
    return static_cast<float>(value - range_min)/(range_max - range_min);
}



}//namespace jetbox
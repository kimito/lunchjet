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
    device.add_listener(EV_KEY, BTN_SOUTH, *this);
}

int RCCarController::listen()
{
    listener.on_change_accel(0);
    listener.on_change_steering(0);

    return device.listen();
}

void RCCarController::on_receive(const struct input_event &event)
{
    switch(event.code) {
        case ABS_X:
            listener.on_change_steering(convert_steering_value(event.value, 0, 65535));
            break;
        case ABS_RZ:
            listener.on_change_accel(convert_accel_value(event.value, 0, 1023));
            break;
        case BTN_SOUTH:
            listener.on_change_back(event.value);
            break;
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

float RCCarController::convert_accel_value(int value, int range_min, int range_max)
{
    return static_cast<float>(value - range_min)/(range_max - range_min);
}



}//namespace jetbox
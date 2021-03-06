#include "rc_car_controller.h"

#include <debug_log.h>

namespace lunchjet {


RCCarController::RCCarController(const std::string &device_file_name,
 RCCarControllerListener &listener,
 std::atomic<bool> &stop_thread)
: device(device_file_name, stop_thread),
  listener(listener)
{
    device.add_listener({
     {EV_ABS, ABS_X},
     {EV_ABS, ABS_RZ},
     {EV_KEY, BTN_SOUTH},
     {EV_KEY, BTN_TL},
     {EV_ABS, ABS_Z}},
     *this);
}

int RCCarController::listen()
{
    listener.on_change_throttle(0);
    listener.on_change_steering(0);
    listener.on_change_brake(0);

    return device.listen();
}

void RCCarController::on_connect()
{
    listener.on_connect();
}

void RCCarController::on_error()
{
    listener.on_error();
}

void RCCarController::on_receive(const struct input_event &event)
{
    switch(event.code) {
        case ABS_X:
            listener.on_change_steering(convert_steering_value(event.value, 0, 65535));
            break;
        case ABS_RZ:
            listener.on_change_throttle(convert_throttle_value(event.value, 0, 1023));
            break;
        case BTN_SOUTH:
            listener.on_change_back(event.value);
            break;
        case BTN_TL:
            if(event.value == 0) { //fire event when the key released
                listener.on_select();
            }
            break;
        case ABS_Z:
            listener.on_change_brake(convert_throttle_value(event.value, 0, 1023));
            break;
        default:
            debug_warning("unknown code:%d value:%d", event.code, event.value);
    }
}

void RCCarController::on_close()
{
    listener.on_close();
}

float RCCarController::convert_steering_value(int value, int range_min, int range_max)
{
    float ave = (range_min + range_max)/2.0f;
    float half_range = (range_max - range_min)/2.0f;

    return (value - ave)/half_range;
}

float RCCarController::convert_throttle_value(int value, int range_min, int range_max)
{
    return static_cast<float>(value - range_min)/(range_max - range_min);
}



}//namespace lunchjet
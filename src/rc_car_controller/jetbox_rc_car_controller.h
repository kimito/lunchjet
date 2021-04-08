#pragma once

#include <string>
#include <atomic>

#include "jetbox_user_input_device.h"

namespace jetbox {

class RCCarControllerListener {
    public:
    virtual void onChangeSteering(float value) = 0;
    virtual void onChangeAccele(float value) = 0;
};

class RCCarController : public UserInputDevice::EventListener {
    public:
    RCCarController(const std::string &device_file_name,
     RCCarControllerListener &listener,
     std::atomic<bool> &stop_thread);
    ~RCCarController() = default;

    int listen();

    void onRecieve(const struct input_event &event) override;

    private:
    UserInputDevice device;
    RCCarControllerListener &listener;

    float convert_steering_value(int value, int range_min, int range_max);
    float convert_accele_value(int value, int range_min, int range_max);
};

}//namespace jetbox
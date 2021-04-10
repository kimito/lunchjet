#pragma once

#include <string>
#include <atomic>

#include "jetbox_user_input_device.h"

namespace jetbox {

class RCCarControllerListener {
    public:
    virtual void on_change_steering(float value) = 0;
    virtual void on_change_accel(float value) = 0;

    /**
     * change status of back button
     * @param[in] value 0 on pressed, 1 on released
     */
    virtual void on_change_back(int value) = 0;
};

class RCCarController : public UserInputDevice::EventListener {
    public:
    RCCarController(const std::string &device_file_name,
     RCCarControllerListener &listener,
     std::atomic<bool> &stop_thread);
    ~RCCarController() = default;

    int listen();

    void on_receive(const struct input_event &event) override;

    private:
    UserInputDevice device;
    RCCarControllerListener &listener;

    float convert_steering_value(int value, int range_min, int range_max);
    float convert_accel_value(int value, int range_min, int range_max);
};

}//namespace jetbox
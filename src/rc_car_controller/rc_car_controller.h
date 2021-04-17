#pragma once

#include <string>
#include <atomic>

#include "lunchjet_user_input_device.h"

namespace lunchjet {

/**
 * a class having call back functions for recieving events about RC car controller
 */
class RCCarControllerListener {
    public:

    /**
     * an event handler for connection with the controller
     */
    virtual void on_connect(){};

    /**
     * an event handler for occurring on the connection
     */
    virtual void on_error(){exit(1);}

    /**
     * an event handler for receiving value of steering
     * @param[in] value direction of steering. -1 for turning left, 1 for tuning right.
     */
    virtual void on_change_steering(float value) = 0;

    /**
     * an event handler for receiving value for accel
     * @param[in] value accel strength, 0 for stopping, 1 for full speed.
     */
    virtual void on_change_accel(float value) = 0;

    /**
     * an event handler for recieving status of the back button
     * @param[in] value status of the back button, 0 with pressed, 1 with released
     */
    virtual void on_change_back(int value) = 0;

    /**
     * an event handler for closing connnection with the controller
     * (the class will retry to wait new connection)
     */
    virtual void on_close(){}
};

/**
 * a class for recieving events from RC car controller device
 */
class RCCarController : public UserInputDevice::EventListener {
    public:
    /**
     * @param[in] device_file_name a file name of a controller devece (e.g., "/dev/input/event1")
     * @param[in] listener a listener to receive input events about the controller
     * @param[in] stop_thread a flag for stopping a thread in this instance
     */
    RCCarController(const std::string &device_file_name,
     RCCarControllerListener &listener,
     std::atomic<bool> &stop_thread);
    ~RCCarController() = default;

    /**
     * start sending the input events to the listener
     * @return >= 0 on success, otherwise error (see errno)
     */
    int listen();

    /**
     * an function for internal use only, do not coll from users.
     */
    virtual void on_connect() override;

    /**
     * an function for internal use only, do not coll from users.
     */
    void on_receive(const struct input_event &event) override;
 
    /**
     * an function for internal use only, do not coll from users.
     */
    void on_close() override;

    /**
     * an function for internal use only, do not coll from users.
     */
    virtual void on_error() override;

    private:
    UserInputDevice device;
    RCCarControllerListener &listener;

    float convert_steering_value(int value, int range_min, int range_max);
    float convert_accel_value(int value, int range_min, int range_max);
};

}//namespace lunchjet
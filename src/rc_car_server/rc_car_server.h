#pragma once

#include <atomic>

#include "rc_car_controller.h"
#include "rc_car_driver.h"
#include "debug_log.h"
#include "string_utils.h"

namespace lunchjet
{


class RCCarServer : public RCCarControllerListener {
    public:
    RCCarServer() = delete;
    RCCarServer(std::atomic<bool> &stop_controller_thread);
    ~RCCarServer() = default;

    /**
     * start processing
     */
    int start();

    //member functions for RCCarControllerListener
    void on_connect() override;
    void on_change_steering(float value) override;
    void on_change_accel(float value) override;
    void on_change_back(int value) override;
    void on_close() override;

    private:
    RCCarDriver driver;
    RCCarController controller;
    bool is_going_back = false;
};

 
} //namespace lunchjet
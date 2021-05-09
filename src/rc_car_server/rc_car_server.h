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
    RCCarServer(std::atomic<bool> &stop_controller_thread) : controller("/dev/input/event2", *this, stop_controller_thread){}
    ~RCCarServer() = default;

    int start() {
        return controller.listen();
    }

    void on_connect(){
        debug_notice("controller connected");
    };

    void on_change_steering(float value) override {
        driver.steer(value);
    }

    void on_change_accel(float value) override {
        if(is_going_back) {
            driver.back(value * 0.3f);
        }
        else {
            driver.go(value * 0.2f);
        }
    }

    void on_change_back(int value) override {
        if(value) {
            is_going_back = true;
        }
        else {
            is_going_back = false;
        }
    }

    void on_close() {
        debug_notice("controller disconnected");
    }

    private:
    RCCarDriver driver;
    RCCarController controller;
    bool is_going_back = false;
};
    
} //namespace lunchjet
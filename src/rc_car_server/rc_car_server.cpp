#include "rc_car_server.h"

namespace lunchjet {

RCCarServer::RCCarServer(std::atomic<bool> &stop_controller_thread)
 : controller("/dev/input/event2", *this, stop_controller_thread)
{
}

int RCCarServer::start() {
    return controller.listen();
}

void RCCarServer::on_connect(){
    debug_notice("controller connected");
};

void RCCarServer::on_change_steering(float value) {
    driver.steer(value);
}

void RCCarServer::on_change_accel(float value) {
    if(is_going_back) {
        driver.back(value * 0.3f);
    }
    else {
        driver.go(value * 0.2f);
    }
}

void RCCarServer::on_change_back(int value) {
    if(value) {
        is_going_back = true;
    }
    else {
        is_going_back = false;
    }
}

void RCCarServer::on_close() {
    debug_notice("controller disconnected");
}

} //namespace lunchjet
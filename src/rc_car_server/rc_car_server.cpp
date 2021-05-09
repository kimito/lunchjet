#include "rc_car_server.h"

#include <cmath>
#include <iostream>

#include "debug_log.h"


namespace lunchjet {

RCCarServer::RCCarServer(std::atomic<bool> &stop_controller_thread)
 : controller("/dev/input/event2", *this, stop_controller_thread),
   video_device(2, [this](auto mat){handle_video(mat);}),
   is_going_back(false),
   is_connected(false),
   steering(0.0f), speed(0.0f)
{
    video_device.start_capture();
}

int RCCarServer::start()
{
    return controller.listen();
}

void RCCarServer::on_connect()
{
    debug_notice("controller connected");
    is_connected = true;
};

void RCCarServer::on_change_steering(float value)
{
    driver.steer(value);

    steering = value;
}

void RCCarServer::on_change_accel(float value)
{    
    if(is_going_back) {
        driver.back(value * 0.3f);
    }
    else {
        driver.go(value * 0.2f);
    }

    speed = value;
}

void RCCarServer::on_change_back(int value)
{
    if(value) {
        is_going_back = true;
    }
    else {
        is_going_back = false;
    }
}

void RCCarServer::on_close()
{
    debug_notice("controller disconnected");
    is_connected = false;
}

void RCCarServer::handle_video(cv::Mat &image)
{
    if(is_connected && std::abs(speed) > 0.1) {
        std::cout << "log image dir: " << LOG_IMAGE_DIR << std::endl;
        std::cout << "log annot dir: " << LOG_ANNOTAION_DIR << std::endl;
    }
}


} //namespace lunchjet
#include "rc_car_server.h"

#include <cmath>
#include <time.h>
#include <fstream>

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

void RCCarServer::on_select()
{
    debug_debug("pressed select button");
}

void RCCarServer::on_close()
{
    debug_notice("controller disconnected");
    is_connected = false;
}

namespace {
    std::string get_log_filename_base() {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME_COARSE, &ts);
        struct tm _tm;
        localtime_r(&(ts.tv_sec), &_tm);

        char time_str[20];
        strftime(time_str, sizeof(time_str), "%Y_%m_%d_%H_%M_%S",&_tm);

        char ms_str[5];
        snprintf(ms_str, sizeof(ms_str), "_%03ld", (ts.tv_nsec / (1000 * 1000)));

        return std::string(time_str) + std::string(ms_str);
    }
}

void RCCarServer::handle_video(cv::Mat &image)
{
    if(!is_connected || std::abs(speed) < 0.1) {
        return;
    }

    std::string file_base = get_log_filename_base();
    std::string image_file_name =  file_base + ".jpg";

    std::ofstream ofs_drive(std::string(LOG_ANNOTAION_DIR) + "/" + file_base + ".log");
    if(!ofs_drive) {
        debug_err("annotation log write failed");
        return;
    }
    ofs_drive << image_file_name << " " << steering << " " << speed << std::endl;

    cv::imwrite(std::string(LOG_IMAGE_DIR) + "/" + image_file_name, image);
}


} //namespace lunchjet
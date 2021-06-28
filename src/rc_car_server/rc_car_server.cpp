#include "rc_car_server.h"

#include <cmath>
#include <time.h>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <algorithm>

#include "debug_log.h"

namespace {
    const std::string MODEL_PATH = "/opt/lunchjet/model.trt";
    const float THROTTLE_GAIN_FORWARD = 0.2f;
    const float THROTTLE_GAIN_BACKWARD = 0.3f;

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


namespace lunchjet {

RCCarServer::RCCarServer(std::atomic<bool> &stop_controller_thread)
 : controller("/dev/input/event2", *this, stop_controller_thread),
   video_device(2, [this](auto mat){handle_video(mat);}),
   is_going_back(false),
   is_connected(false),
   steering(0.0f), throttle(0.0f),
   throttle_magnification(THROTTLE_GAIN_FORWARD),
   is_manual_drived(true),
   detector(MODEL_PATH)
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
    if(is_manual_drived) {
        driver.steer(value);

        steering = value;
    }
}

void RCCarServer::on_change_throttle(float value)
{
    if(is_manual_drived) {
        if(is_going_back) {
            driver.back(value * 0.3f);
        }
        else {
            driver.go(value * throttle_magnification);
        }

        throttle = value;
    }
}

void RCCarServer::on_change_brake(float value)
{
    //when not braking(value = 0) , throttle_magnification bust be THROTTLE_GAIN_FORWARD
    //when full braking(value = 1), throttle_magnification must be 0
    throttle_magnification = THROTTLE_GAIN_FORWARD * (1 - value);
}

void RCCarServer::on_change_back(int value)
{
    if(is_manual_drived) {
        if(value) {
            is_going_back = true;
        }
        else {
            is_going_back = false;
        }
    }
}

void RCCarServer::upload_control_data()
{
#if 0
        //upload training data to Google Cloud
        debug_notice("stating upload control logs to Google drive...");
        std::string shell_param = "PYTHONPATH=" + vars["PYTHONPATH"] + 
            " python3 /opt/lunchjet/scripts/upload_training_data.py"
            " --root /var/log/lunchjet --rm /var/log/lunchjet";
        debug_notice("launch : %s", shell_param.c_str());
        int res = system(shell_param.c_str());
        if(res == 0) {
            debug_notice("finish uploading");
        }
        else {
            debug_err("uploading failed : %s", strerror(errno));
        }
#endif
}


void RCCarServer::on_select()
{
    debug_debug("pressed select button");

    //flip the mode
    is_manual_drived = !is_manual_drived;

    if(!is_manual_drived) {
        debug_notice("now autonomous mode");
        upload_control_data();
    }
    else {
       debug_notice("now manual mode");
       throttle_magnification = THROTTLE_GAIN_FORWARD;

        this->on_change_steering(0.0f);
        this->on_change_throttle(0.0f);
    }
}

void RCCarServer::on_close()
{
    debug_notice("controller disconnected");
    is_connected = false;
}

void RCCarServer::record_control_data(cv::Mat &image)
{
    if(!is_connected || std::abs(throttle) < 0.1) {
        return;
    }

    std::string file_base = get_log_filename_base();
    std::string image_file_name =  file_base + ".jpg";

    std::ofstream ofs_drive(std::string(LOG_ANNOTAION_DIR) + "/" + file_base + ".log");
    if(!ofs_drive) {
        debug_err("annotation log write failed");
        return;
    }
    ofs_drive << image_file_name << " " << steering << " " << throttle << std::endl;

    cv::imwrite(std::string(LOG_IMAGE_DIR) + "/" + image_file_name, image);

}

void RCCarServer::handle_video(cv::Mat &image)
{
    if(is_manual_drived) {
        record_control_data(image);
    }
    else {
        auto params = detector.detect(image);

        driver.steer(params.steering);
        driver.go(params.throttle * throttle_magnification);
    }
}


} //namespace lunchjet
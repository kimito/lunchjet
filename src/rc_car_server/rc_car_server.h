#pragma once

#include <atomic>
#include <memory>

#include "rc_car_controller.h"
#include "rc_car_driver.h"
#include "video_input_device.h"
#include "debug_log.h"
#include "string_utils.h"
#include "setting_variables.h"
#include "drive_detector.h"

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
    void on_select() override;
    void on_close() override;

    private:
    RCCarDriver driver;
    RCCarController controller;
    VideoInputDevice video_device;
    bool is_going_back;
    std::atomic<bool> is_connected;
    std::atomic<float> steering, speed;
    bool is_manual_drived;
    SettingVariables vars;
    std::unique_ptr<DriveDetector> detector;

    void handle_video(cv::Mat &image);    
    void record_control_data(cv::Mat &image);
    void upload_control_data();
};

 
} //namespace lunchjet
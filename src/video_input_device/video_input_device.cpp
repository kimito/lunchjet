#include "video_input_device.h"

#include <iostream>
#include <opencv2/videoio.hpp>

#include "debug_log.h"


namespace lunchjet
{

VideoInputDevice::VideoInputDevice(int device_index, ProcessingFunction func)
: device_index(device_index), func(func), stop_thread(false)
{
}

bool VideoInputDevice::start_capture()
{
    stop_thread = false;
    thread = std::thread([this]{main_loop();});

    return true;
}

bool VideoInputDevice::stop_capture()
{
    stop_thread = true;
    if(thread.joinable()) {
        thread.join();
    }

    return true;
}

VideoInputDevice::~VideoInputDevice()
{
    stop_capture();
}

void VideoInputDevice::main_loop()
{
    debug_notice("main loop start");

    cv::VideoCapture device(device_index);
    cv::Mat mat;

    while(!stop_thread.load()) {
        if(device.grab()) {
            device.retrieve(mat);

            func(mat);
        }
    }

    debug_notice("main loop end");
}


} //namespace lunchjet
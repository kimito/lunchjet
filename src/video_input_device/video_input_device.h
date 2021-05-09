#pragma once

#include <functional>
#include <thread>
#include <atomic>
#include <opencv2/opencv.hpp>

namespace lunchjet {

class VideoInputDevice {
    public:
    /**
     * function type for event handle with saved a image
     * frist argument(const std::string&) is for file name
     * second argment(bool) is for whether processing was successful
     */
    using ProcessingFunction = std::function<void(cv::Mat &)>;

    VideoInputDevice() = delete;

    /**
     * @param[in] device_index N in /dev/videoN
     */
    VideoInputDevice(int device_index, ProcessingFunction func);

    ~VideoInputDevice();

    bool start_capture();
    bool stop_capture();

    private:

    void main_loop();

    int device_index;
    ProcessingFunction func;
    std::thread thread;
    std::atomic<bool> stop_thread;
};

} //namespace lunchjet
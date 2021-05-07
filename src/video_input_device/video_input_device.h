#pragma once

#include <functional>
#include <string>
#include <opencv2/videoio.hpp>


namespace lunchjet {

/**
 * a class represents a video input device
 */
class VideoInputDevice {
    public:

    /**
     * event handler for saving a image.
     * std::stirng is for file name of the image,
     * and bool is for the processing result, which is true on success
     */
    using SaveEventHandler = typename std::function<void(const std::string&, bool)>;

    /**
     * @param[in] camera_index index of a camera device (N of /dev/videoN)
     */
    VideoInputDevice(int camera_index);

    VideoInputDevice() = delete;
    ~VideoInputDevice() = default;

    /**
     * save a image from the device.
     * this function will return before saving, and will call member function of a event handler on finish processing
     * @param[in] file_name file name of the image
     * @param[in] handler the event handler
     */
    void save_image(const std::string &file_name, SaveEventHandler handler);

    private:

    cv::VideoCapture device;
};


} //namespace lunchjet
#include "video_input_device.h"

#include <opencv2/imgcodecs.hpp>


namespace lunchjet {

VideoInputDevice::VideoInputDevice(int camera_index)
: device(camera_index)
{

}

void VideoInputDevice::save_image(const std::string &file_name, SaveEventHandler handler)
{
    cv::Mat image;

    if(!device.isOpened() || !device.read(image) || !cv::imwrite(file_name, image)) {
        handler(file_name, false);
        return;
    }

    handler(file_name, true);

    return;
}


} //namespace lunchjet
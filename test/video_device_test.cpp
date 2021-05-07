#include <iostream>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "performance_count.h"


int main(int argc, const char *argv[])
{
    int device_num = 2;
    cv::VideoCapture video(device_num);

    if( !video.isOpened() ) {
        std::cerr << "cannot open video device: " << device_num << std::endl;
        return -1;
    }

    cv::Mat image;

    for(int i = 0; i < 10; ++i)
    {
        auto start = time_now();
        if( !video.read(image) ) {
            std::cerr << "cannot grab any image" << std::endl;
            return -1;
        }
        std::cout << "time to read: " << duration_ms_from(start) << std::endl;
    }

    cv::imwrite("image.jpg", image);

    return 0;
}

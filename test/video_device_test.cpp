#include <iostream>

#include "video_input_device.h"

using namespace lunchjet;

int main(int argc, const char *argv[])
{
    int device_num = 2;
    VideoInputDevice device(device_num);

    cv::Mat image;

    for(int i = 0; i < 10; ++i)
    {
        device.save_image("image" + std::to_string(i)  + ".jpg",
        [](auto file_name, auto result){std::cout << "complete: " << file_name << std::endl;});
    }

    return 0;
}

#include <iostream>
#include <chrono>
#include <thread>

#include "video_input_device.h"
#include "string_utils.h"

using namespace lunchjet;

int main(int argc, const char *argv[])
{
    std::cout << "start " << std::string(argv[0]) << std::endl;

    VideoInputDevice device(2,
    [](auto mat){std::cout << "complete: " << mat.cols << "x" << mat.rows  << std::endl;});

    if(!device.start_capture()) {
        std::cerr << "start_capture() failed: " << errno2str() << std::endl;
        return -1;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}

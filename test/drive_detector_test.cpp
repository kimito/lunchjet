#include <iostream>

#include "drive_detector.h"

using namespace lunchjet;

int main(int argc, char *argv[])
{
    if(argc < 3) {
        std::cerr << "usage: " << argv[0] << "MODEL_FILE_NAME INPUT_IMAGE_FILENAME" << std::endl;
        return -1;
    }

    DriveDetector dcon(argv[1]);
    cv::Mat image = cv::imread(argv[2]);

    auto params = dcon.detect(image);

    std::cout << "parameters: steer=" << params.steering << " throtle=" << params.throtle << std::endl;
    return 0;
}
#include <iostream>

#include "drive_detector.h"

#include "performance_count.h"

using namespace lunchjet;

int main(int argc, char *argv[])
{
    if(argc < 3) {
        std::cerr << "usage: " << argv[0] << "MODEL_FILE_NAME INPUT_IMAGE_FILENAME" << std::endl;
        return -1;
    }

    DriveDetector dcon(argv[1]);
    cv::Mat image = cv::imread(argv[2]);

    for(int i = 0; i < 10; ++i) {
        auto infer_start = time_now();
        
        auto params = dcon.detect(image);

        std::cout << "params: steer=" << params.steering
         << " throtle=" << params.throtle
         << " " << duration_ms_from(infer_start) << "ms" << std::endl;
    }
    return 0;
}
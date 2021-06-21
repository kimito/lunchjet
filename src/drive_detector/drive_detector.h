#pragma once

#include <string>

#include <opencv2/opencv.hpp>


namespace lunchjet {

struct DriveParameter {
    float steering;
    float throtle;
};

class DriveDetector {
    public:

    DriveDetector(const std::string &model_file_path)
        : model_file_path(model_file_path) {}
    ~DriveDetector() = default;

    DriveParameter detect(const cv::Mat &image);

    private:
    std::string model_file_path;
};

} //namespace lunchjet
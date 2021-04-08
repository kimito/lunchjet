#pragma once

#include <string>
#include <atomic>

namespace jetbox {

class RCCarControllerListener {
    public:
    virtual void onChangeSteering(float value) = 0;
    virtual void onChangeAccele(float value) = 0;
};

class RCCarController {
    public:
    RCCarController(const std::string &device_file_name, RCCarControllerListener &listerner);
    ~RCCarController() = default;

    int listen(std::atomic<bool> &stop_thread);
};

}//namespace jetbox
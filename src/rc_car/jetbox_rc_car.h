#pragma once

#include "jetbox_pca9685.h"

namespace jetbox {

class RCCar {
    public:
    RCCar();
    ~RCCar() = default;

    /**
     * set steering value
     * @param[in] value from -1.0 to 1.0. -1.0 means turning left. 1.0 means turning right.
     */
    int steer(float value);

    /**
     * go forward with a specific speed
     * @param[in] speed from 0.0 to 1.0. 0.0 means stopping. 1.0 means going with full speed.
     */
    int go(float speed);

    private:
    PCA9685 driver;
};

}//namespace jetbox
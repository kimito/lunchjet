#pragma once

#include "pca9685.h"

namespace lunchjet {

/**
 * a class to drive RC car
 */ 
class RCCarDriver {
    public:
    RCCarDriver();
    ~RCCarDriver();

    /**
     * set steering value
     * @param[in] value from -1.0 to 1.0. -1.0 means turning left. 1.0 means turning right.
     * @return 0 on success, otherwise error (see errno)
     */
    int steer(float value);

    /**
     * go forward with a specific speed (memo: 0.084 is seemed to be a threshold to actual move)
     * @param[in] speed from 0.0 to 1.0. 0.0 means stopping. 1.0 means going with full speed.
     * @return 0 on success, otherwise error (see errno)
     */
    int go(float speed);

    /**
     * go backward with a specific speed
     * @param[in] speed from 0.0 to 1.0. 0.0 means stopping. 1.0 means going with full speed.
     * @return 0 on success, otherwise error (see errno)
     */
    int back(float speed);

    /**
     * stop the drive motor
     * @return 0 on success, otherwise error (see errno)
     */
    int stop();

    private:
    PCA9685 driver;
};

}//namespace lunchjet
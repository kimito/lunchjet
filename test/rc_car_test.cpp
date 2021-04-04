#include "jetbox_rc_car.h"

#include <iostream>
#include <thread>
#include <cmath>

using namespace jetbox;

int main(int argc, const char *argv[])
{
    RCCar car;

    for(float s = 0.f; s <= 5.f; s+=0.05f) {
        car.steer(std::sin(2*M_PI*s));
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return 0;
}
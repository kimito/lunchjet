#include "jetbox_rc_car.h"

#include <iostream>
#include <thread>
#include <cmath>

using namespace jetbox;

int main(int argc, const char *argv[])
{
    RCCar car;
#if 0
    //swing the steering
    for(float s = 0.f; s <= 3.f; s+=0.05f) {
        car.steer(std::sin(2*M_PI*s));
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
#endif
    
    std::this_thread::sleep_for(std::chrono::seconds(3));
    car.go(0.138f);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    car.go(0.0f);

    return 0;


    //stop and go
    for(float s = 0.f; s <= 1.f; s+=0.05f) {
        car.go((-std::cos(2*M_PI*s)+1)/2);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
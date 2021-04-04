#include "jetbox_pca9685.h"

#include <iostream>
#include <thread>

using namespace jetbox;

int main(int argc, const char *argv[])
{
    PCA9685 driver(17.5 * 1000);

    driver.set_pulse(0, 1.5 * 1000);
    driver.set_pulse(1, 1.5 * 1000);

    driver.start();
    std::this_thread::sleep_for(std::chrono::seconds(2));

    //servo motor control
    //turn left
    driver.set_pulse(0, 1.1 * 1000);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    //neutral
    driver.set_pulse(0, 1.5 * 1000);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    //turn right
    driver.set_pulse(0, 1.9 * 1000);
    std::this_thread::sleep_for(std::chrono::seconds(2));

    //neutral
    driver.set_pulse(0, 1.5 * 1000);
    std::this_thread::sleep_for(std::chrono::seconds(2));


    //drive mortor control
    //move backward
    driver.set_pulse(1, 1.9 * 1000);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    driver.set_pulse(1, 1.5 * 1000);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    driver.set_pulse(1, 1.6 * 1000);

    //neutral
    std::this_thread::sleep_for(std::chrono::seconds(2));
    driver.set_pulse(1, 1.5 * 1000);

    //move forward
    std::this_thread::sleep_for(std::chrono::seconds(2));
    driver.set_pulse(1, 1.45 * 1000);

    //neutral
    std::this_thread::sleep_for(std::chrono::seconds(2));
    driver.set_pulse(1, 1.5 * 1000);

    // std::this_thread::sleep_for(std::chrono::seconds(3));
    // driver.reset();

    return 0;
}
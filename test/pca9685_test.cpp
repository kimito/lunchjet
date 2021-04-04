#include "jetbox_pca9685.h"

#include <iostream>
#include <thread>

using namespace jetbox;

int main(int argc, const char *argv[])
{
    PCA9685 driver(17.25 * 1000);

    driver.set_pulse(0, 1.5 * 1000);
    driver.set_pulse(1, 1.5 * 1000);

//    driver.start();

    std::this_thread::sleep_for(std::chrono::seconds(3));

    driver.set_pulse(0, 1.1 * 1000);
    driver.set_pulse(1, 1.1 * 1000);

    std::this_thread::sleep_for(std::chrono::seconds(3));

    driver.set_pulse(0, 1.9 * 1000);
    driver.set_pulse(1, 1.9 * 1000);

    return 0;
}
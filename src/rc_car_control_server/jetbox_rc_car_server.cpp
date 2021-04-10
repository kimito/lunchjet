#include <iostream>

#include <iostream>
#include <atomic>
#include <signal.h>
#include <string.h>
#include <unistd.h>


#include "jetbox_rc_car_driver.h"
#include "jetbox_rc_car_controller.h"

using namespace jetbox;

namespace {

std::atomic<bool> stop_controller_thread(false);

void signal_handler(int signal) {
    stop_controller_thread.store(true);
}

}


using namespace jetbox;
class RCCarServer : public RCCarControllerListener {
    public:
    RCCarServer() : controller("/dev/input/event3", *this, stop_controller_thread){}
    ~RCCarServer() = default;

    int start() {
        controller.listen();
    }

    void on_change_steering(float value) override {
        driver.steer(value);
    }

    void on_change_accel(float value) override {
        if(is_going_back) {
            driver.back(value * 0.2f);
        }
        else {
            driver.go(value * 0.2f);
        }
    }

    void on_change_back(int value) override {
        if(value) {
            is_going_back = true;
        }
        else {
            is_going_back = false;
        }
    }

    private:
    RCCarDriver driver;
    RCCarController controller;
    bool is_going_back = false;
};

int main(int argc, const char *argv[])
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sigfillset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    RCCarServer server;

    std::cout << "start server" << std::endl;
    server.start();
    pause();
    std::cout << "exit server" << std::endl;

    return 0;
}
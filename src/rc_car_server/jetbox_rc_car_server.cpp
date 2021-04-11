#include <iostream>

#include <iostream>
#include <atomic>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <thread>

#include "jetbox_rc_car_driver.h"
#include "jetbox_rc_car_controller.h"
#include "debug_log.h"
#include "string_utils.h"

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
        return controller.listen();
    }

    void on_change_steering(float value) override {
        driver.steer(value);
    }

    void on_change_accel(float value) override {
        if(is_going_back) {
            driver.back(value * 0.3f);
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

    std::cout << "starting server..." << std::endl;
    while(true) {
        int ret = server.start();
        if(ret >= 0) {
            break;
        }
        else if(errno == ENOENT){
            debug_notice("controller not found retry after 3sec...");
            std::this_thread::sleep_for(std::chrono::seconds(3));
            continue;
        }
        else {
            std::cerr << "error occurred on starting the server :" << errno2str() << std::endl;
            exit(1);
        }
    }

    std::cout << "server started" << std::endl;
    pause();
    std::cout << "terminating server..." << std::endl;

    return 0;
}
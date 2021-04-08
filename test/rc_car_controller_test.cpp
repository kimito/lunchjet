#include "jetbox_rc_car_controller.h"

#include <iostream>
#include <linux/input.h>
#include <linux/input-event-codes.h>
#include <atomic>
#include <signal.h>
#include <string.h>
#include <unistd.h>


using namespace jetbox;

namespace {

std::atomic<bool> stop_controller_thread(false);

void signal_handler(int signal) {
    stop_controller_thread.store(true);
}

}

using namespace jetbox;

class ControllerListener : public RCCarControllerListener {
    void onChangeSteering(float value) override {
        std::cout << "steering:" << value << std::endl;
    }

    void onChangeAccele(float value) override {
        std::cout << "accele:" << value << std::endl;
    }
};

int main(int argc, const char *argv[])
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sigfillset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    ControllerListener listener;
    RCCarController controller("/dev/input/event3", listener);

    if(controller.listen(stop_controller_thread) < 0) {
        perror("listen() failed");
        exit(1);
    }

    std::cout << "hello world" << std::endl;
    pause();    

    return 0;   
}
#include "jetbox_user_input_device.h"

#include <iostream>
#include <linux/input.h>
#include <linux/input-event-codes.h>
#include <atomic>
#include <signal.h>
#include <string.h>
#include <unistd.h>


using namespace jetbox;

namespace {

std::atomic<bool> stop_user_input_device_thread(false);

void signal_handler(int signal) {
    stop_user_input_device_thread.store(true);
}

struct sigaction sa;
}


class Listener : public UserInputDevice::EventListener {
    void onRecieve(const struct input_event &event) override {
        std::cout << "event type:" << event.type << " code:" << event.code << " value:" << event.value << std::endl;
    }
};

int main(int argc, const char *argv[])
{
    std::cout << "hello world!" << std::endl;

    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sigfillset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    UserInputDevice device("/dev/input/event3", stop_user_input_device_thread);
    Listener listener;

    device.add_listener(EV_ABS, ABS_RX, listener);
    if(device.listen() == -1){
        std::cout << "listen() failed" << std::endl;
    }

    pause();
    std::cout << "exit main()" << std::endl;

    return 0;
}
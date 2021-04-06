#include "jetbox_user_input_device.h"

#include <iostream>
#include <linux/input.h>
#include <linux/input-event-codes.h>

using namespace jetbox;

class Listener : public UserInputDevice::EventListener {
    void onRecieve(const struct input_event &event) override {
        std::cout << "event type:" << event.type << " code:" << event.code << " value:" << event.value << std::endl;
    }
};

int main(int argc, const char *argv[])
{
    std::cout << "hello world!" << std::endl;

    UserInputDevice device("/dev/input/event3");
    Listener listener;

    device.add_listener(EV_ABS, ABS_RX, listener);
    device.listen();

    return 0;
}
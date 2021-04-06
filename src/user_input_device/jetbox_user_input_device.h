#pragma once

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <linux/input.h>

namespace jetbox {


class UserInputDevice {
    public:
    UserInputDevice(const std::string &file_name);
    ~UserInputDevice();

    class EventListener {
        public:
        virtual void onRecieve(const struct input_event &event) = 0;
    };
    void add_listener(uint16_t type, uint16_t code, EventListener &listener); 

    int listen();

    private:

    struct EventFiler {

    };

    std::vector<EventListener> listeners;
    std::mutex listeners_mutex;
    std::thread waiting_thread;
    volatile bool stop_thread;
};

}//namespace jetbox
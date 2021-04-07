#pragma once

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <stdint.h>
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
    struct EventFilter {
        uint16_t type;
        uint16_t code;
        EventListener &listener;
    };

    std::string file_name;

    std::vector<EventFilter> filters;
    std::mutex mutex_filters;

    int fd;

    std::thread waiting_thread;
    volatile bool stop_thread;

    void listen_thread();
};

}//namespace jetbox
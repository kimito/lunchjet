#pragma once

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <stdint.h>
#include <linux/input.h>
#include <atomic>

namespace jetbox {


class UserInputDevice {
    public:
    UserInputDevice(const std::string &file_name, const std::atomic<bool> &stop_thread);
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
    const std::atomic<bool> &stop_thread;

    void listen_thread();
    void forward_events(const struct input_event *events, int num_events);
};

}//namespace jetbox
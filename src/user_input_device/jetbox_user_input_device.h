#pragma once

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <stdint.h>
#include <linux/input.h>
#include <atomic>

namespace jetbox {

/**
 * a class that represents an user input device
 * @see https://www.kernel.org/doc/html/latest/input/input_uapi.html
 */
class UserInputDevice {
    public:
    /**
     * @param[in] file_name device file name
     * @param[in] stop_thread a flag to stop receiving the input events
     */
    UserInputDevice(const std::string &file_name, const std::atomic<bool> &stop_thread);
    ~UserInputDevice();

    class EventListener {
        public:
        /**
         * an event handler for receiving input events
         * @param[in] event an input event
         */
        virtual void on_receive(const struct input_event &event) = 0;
    };

    /**
     * add a listener to receive input events
     * @param[in] type the type of the event that the listener wants to receive (input_event::type)
     * @param[in] code the code of the event that the listener wants to receive (input_event::type)
     * @param[in] listener the listener
     */ 
    void add_listener(uint16_t type, uint16_t code, EventListener &listener); 

    /**
     * start receiving input events for the listener
     * @return >= 0 on success, otherwise error (see errno)
     */
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
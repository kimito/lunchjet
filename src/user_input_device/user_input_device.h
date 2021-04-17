#pragma once

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <stdint.h>
#include <linux/input.h>
#include <atomic>

namespace lunchjet {

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
         * an event handler for starting connection with the device
         */
        virtual void on_connect(){}

        /**
         * an event handler for receiving input events
         * @param[in] event an input event
         */
        virtual void on_receive(const struct input_event &event) = 0;

        /**
         * an event handler for closing connection with the device
         */
        virtual void on_close(){}

        /**
         * an event handler for occurring some error that can't be fixed (see errno for detail)
         * after calling this function, the class will stop working.
         */
        virtual void on_error(){exit(1);}
    };

    struct EventIndicator {
        uint16_t type; //!< the type of the event. (see struct input_event::type)
        uint16_t code; //!< the code of the event. (see struct input_event::type)
    };

    /**
     * add a listener to receive input events
     * @param[in] indicators 
     * @param[in] listener the listener
     */ 
    void add_listener(const std::vector<EventIndicator> &indicators, EventListener &listener); 

    /**
     * start receiving input events for the listener
     * @return >= 0 on success, otherwise error (see errno)
     */
    int listen();

    private:
    struct EventFilter {
        std::vector<EventIndicator> indicators;
        EventListener &listener;
    };

    std::string file_name;

    std::vector<EventFilter> filters;
    std::mutex mutex_filters;

    int fd;

    std::thread waiting_thread;
    const std::atomic<bool> &stop_thread;

    void listen_thread();
    void notify_connect();
    void notify_error();
    void forward_events(const struct input_event *events, int num_events);
    void notify_close();
};

}//namespace lunchjet
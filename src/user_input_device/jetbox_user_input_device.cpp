#include "jetbox_user_input_device.h"

#include <iostream>


namespace jetbox {


UserInputDevice::UserInputDevice(const std::string &file_name)
: file_name(),
  fd(-1),
  stop_thread(false) {}

UserInputDevice::~UserInputDevice()
{
    stop_thread = true;
    if(waiting_thread.joinable()){
        waiting_thread.join();
    }
}

void UserInputDevice::add_listener(uint16_t type, uint16_t code, EventListener &listener)
{
    std::lock_guard<std::mutex> guard(mutex_filters);
    filters.push_back({type, code, listener});
}

int UserInputDevice::listen()
{
    waiting_thread = std::thread([this](){listen_thread();});
}

void UserInputDevice::listen_thread()
{
    while(!stop_thread) {
        std::cout << "listen_thread() start" << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "listen_thread() end" << std::endl;
}


} //namespace jetbox
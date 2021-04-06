#include "jetbox_user_input_device.h"

namespace jetbox {


UserInputDevice::UserInputDevice(const std::string &file_name)
: stop_thread(false) {}

UserInputDevice::~UserInputDevice()
{
    stop_thread = true;
    if(waiting_thread.joinable()){
        waiting_thread.join();
    }
}

void UserInputDevice::add_listener(uint16_t type, uint16_t code, EventListener &listener)
{

}

int UserInputDevice::listen()
{

}


} //namespace jetbox
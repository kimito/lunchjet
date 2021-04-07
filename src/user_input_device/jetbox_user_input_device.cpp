#include "jetbox_user_input_device.h"

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include "debug_log.h"
#include "string_utils.h"


namespace jetbox {


UserInputDevice::UserInputDevice(const std::string &file_name, const std::atomic<bool> &stop_thread)
: file_name(file_name),
  fd(-1),
  stop_thread(stop_thread){}

UserInputDevice::~UserInputDevice()
{
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
    fd = open(file_name.c_str(), O_RDONLY);
    if(fd < 0) {
        debug_err("user input device '%s' open() failed; %s", file_name.c_str(), errno2str().c_str());
        if(errno == EACCES && getuid() != 0) {
            debug_err("please run as root user");
        }
        return fd;
    }

    //set signal masks for the child thread
    sigset_t block, old_block;
    sigemptyset(&block);
    sigaddset(&block, SIGINT);
    sigaddset(&block, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &block, &old_block);

    waiting_thread = std::thread([this](){listen_thread();});

    //set original signal masks back
    pthread_sigmask(SIG_SETMASK, &old_block, NULL);
}

void UserInputDevice::listen_thread()
{
    std::cout << "listen_thread() start" << std::endl;

    while(!stop_thread.load()) {
        std::cout << "listen_thread() wait..." << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    close(fd);

    std::cout << "listen_thread() end" << std::endl;
}


} //namespace jetbox
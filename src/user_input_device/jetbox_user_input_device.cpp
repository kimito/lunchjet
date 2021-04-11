#include "jetbox_user_input_device.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>
#include <thread>

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

void UserInputDevice::add_listener(const std::vector<EventIndicator> &indicators, EventListener &listener)
{
    std::lock_guard<std::mutex> guard(mutex_filters);
    filters.push_back({indicators, listener});
}

int UserInputDevice::listen()
{
    //set signal masks for the child thread
    sigset_t block, old_block;
    sigemptyset(&block);
    sigaddset(&block, SIGINT);
    sigaddset(&block, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &block, &old_block);

    waiting_thread = std::thread([this](){listen_thread();});

    //set original signal masks back
    pthread_sigmask(SIG_SETMASK, &old_block, NULL);

    return 0;
}

void UserInputDevice::notify_connect()
{
    for(auto &filter : filters) {
        filter.listener.on_connect();
    }
}

void UserInputDevice::notify_error()
{
    for(auto &filter : filters) {
        filter.listener.on_error();
    }
}


void UserInputDevice::forward_events(const struct input_event *events, int num_events)
{
    for(int i = 0; i < num_events; ++i) {
        const struct input_event &event = events[i];

        for(auto &filter : filters){
            for(auto &indicator : filter.indicators) {
                if(event.type == indicator.type && event.code == indicator.code) {
                    filter.listener.on_receive(event);
                    break;
                }
            }
        }
    }
}

void UserInputDevice::notify_close()
{
    for(auto &filter : filters) {
        filter.listener.on_close();
    }
}


void UserInputDevice::listen_thread()
{    
    struct input_event events[64];
    bool error_occurred = false;

    while(!stop_thread.load() && !error_occurred) {
        if((fd = open(file_name.c_str(), O_RDONLY)) < 0) {
            if(errno == ENOENT) {
                std::this_thread::sleep_for(std::chrono::seconds(3));
                continue;
            }
            else {
                debug_err("user input device '%s' open() failed; %s", file_name.c_str(), errno2str().c_str());
                notify_error();
                break;
            }
        }
        notify_connect();

        //loop for select() and read()
        while(!stop_thread.load()) {
            fd_set read_fds;
            FD_ZERO(&read_fds);
            FD_SET(fd, &read_fds);
            struct timeval time_out = {1, 0};//1 second
            int ret = select(fd + 1, &read_fds, NULL, NULL, &time_out);
            if(stop_thread.load()) {
                debug_notice("requested to stop");
                break;
            }

            if(ret == -1) {
                if(errno == EBADF) {
                    debug_notice("device closed");
                    notify_close();
                    break;
                }
                else {
                    debug_err("select() failed: %s", errno2str().c_str());
                    error_occurred = true;
                    notify_error();
                    break;
                }
            }
            else if(ret == 0) { //select() exited with time out
                continue;
            }

            if((ret = read(fd, events, sizeof(events))) < sizeof(struct input_event)) {
                debug_err("read() failed: %s", errno2str().c_str());
                continue;
            }

            forward_events(events, ret/sizeof(struct input_event));
        }

        close(fd);
        fd = -1;
    }

    close(fd);

    debug_notice("listen_thread() end");
}


} //namespace jetbox
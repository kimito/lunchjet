#include <atomic>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <thread>

#include "rc_car_server.h"

using namespace lunchjet;

namespace {

std::atomic<bool> stop_controller_thread(false);

void signal_handler(int signal) {
    stop_controller_thread.store(true);
}

}

int main(int argc, const char *argv[])
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sigfillset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    RCCarServer server(stop_controller_thread);

    debug_notice("starting server...");

    server.start();
    debug_notice("server started");

    pause();
    debug_notice("terminating server...");

    return 0;
}
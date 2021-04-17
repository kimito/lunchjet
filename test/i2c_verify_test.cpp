#include <iostream>
#include <iomanip>
#include <syslog.h>
#include <string.h>

#include "lunchjet_i2c.h"
#include "string_utils.h"

using namespace lunchjet;

int main(int argc, const char *argv[])
{
    //
    // This program tests I2C by doing read-write-verify with lunchjet::I2CDevice
    //

    if(argc < 5) {
        std::cout << "usage: " << std::string(argv[0])
         << " ADAPTER_NUM DEVICE_ADDRESS REGISTER_ADDRESS REGISTER_VALUE" << std::endl;
        exit(1);
    }

    uint8_t adapter_num = str2hex_u8(argv[1]);
    uint8_t device_address = str2hex_u8(argv[2]);
    uint8_t register_address = str2hex_u8(argv[3]);
    uint8_t register_value = str2hex_u8(argv[4]);

    openlog("i2c_test", LOG_CONS | LOG_PID, LOG_USER);
    std::cout << "adp:" << hex2str(adapter_num) << " dev:" << hex2str(device_address)
     << " reg:" << hex2str(register_address) << " val:" << hex2str(register_value) << std::endl;

    auto device = I2CDevice(adapter_num, device_address);

    //read
    uint8_t read_value;
    if(device.read(register_address, read_value) > 0) {
        std::cout << "read register value " << hex2str(read_value) << std::endl;
    }
    else {
        std::cout << "read some error occurred: " << errno2str() << std::endl;
        exit(1);
    }

    //write
    if(device.write(register_address, register_value) > 0) {
        std::cout << "write register value " << hex2str(register_value) << std::endl;
    }
    else {
        std::cout << "write some error occurred: " << errno2str() << std::endl;
        exit(1);

    }

    //read again
    if(device.read(register_address, read_value) > 0) {
        std::cout << "read register value " << hex2str(read_value) << std::endl;
    }
    else {
        std::cout << "read some error occurred: " << errno2str() << std::endl;
        exit(1);
    }

    //verify
    if(register_value == read_value) {
        std::cout << "verify success" << std::endl;
    }
    else {
        std::cout << "verify failed" << std::endl;
        exit(1);
    }

    return 0;
}
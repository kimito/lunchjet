#pragma once

#include <string>
#include <sstream>
#include <iomanip>
#include <string.h>

namespace jetbox {

/**
 * convers an hexadecimal value to an string
 * @param[in] value the hexadecimal value
 * @return the string
 */
template<typename T> std::string hex2str(T &&value)
{
    std::stringstream ss_num;
    ss_num << std::hex << std::setfill('0') << std::setw(sizeof(T)*2) << std::uppercase << static_cast<int>(value);

    std::stringstream ss;
     ss << "0x" << ss_num.str();
    return ss.str();
}

/**
 * gets an string associated with an errno
 */
inline std::string errno2str()
{
    return std::string(strerror(errno));
}

/**
 * converts an string that represents an number to 8 bit number
 * @param[in] str the string
 * @return the number
 */
inline uint8_t str2hex_u8(const std::string &str)
{
    return static_cast<uint8_t>(std::stoi(str, nullptr, 0));
}

/**
 * converts an string that represents an number to 8 bit number
 * @param[in] str the string
 * @return the number
 */
inline uint8_t str2hex_u8(const char *str)
{
    return str2hex_u8(std::string(str));
}


} //namespace jetbox

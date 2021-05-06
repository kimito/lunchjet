#pragma once

#include <chrono>

/**
 * get time point of now
 */
inline std::chrono::system_clock::time_point time_now()
{
    return std::chrono::system_clock::now();
}

/**
 * return duration in millisecconds until now from a ceartain time point
 * @param[in] start point of the duration
 * @return the duration in milliseconds
 */
inline auto duration_ms_from(std::chrono::system_clock::time_point &from)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(time_now() - from).count();
}
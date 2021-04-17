cmake_minimum_required(VERSION 3.10)
project("LunchJet User Input Device")

add_library(lunchjet_user_input_device STATIC lunchjet_user_input_device.cpp)
target_link_libraries(lunchjet_user_input_device lunchjet_include pthread)
target_include_directories(lunchjet_user_input_device PUBLIC .)
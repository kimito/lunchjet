cmake_minimum_required(VERSION 3.10)
project("JetBox User Input Device")

add_library(jetbox_user_input_device STATIC jetbox_user_input_device.cpp)
target_link_libraries(jetbox_user_input_device pthread)
target_include_directories(jetbox_user_input_device PUBLIC .)
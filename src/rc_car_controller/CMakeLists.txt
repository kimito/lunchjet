cmake_minimum_required(VERSION 3.10)
project("JetBox User Input Device")

add_library(jetbox_rc_car_controller STATIC jetbox_rc_car_controller.cpp)
target_link_libraries(jetbox_rc_car_controller jetbox_include jetbox_user_input_device)
target_include_directories(jetbox_rc_car_controller PUBLIC .)
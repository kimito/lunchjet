cmake_minimum_required(VERSION 3.10)
project("JetBox RC Car")

add_library(jetbox_rc_car_driver STATIC jetbox_rc_car_driver.cpp)
target_link_libraries(jetbox_rc_car_driver jetbox_pca9685)
target_include_directories(jetbox_rc_car_driver PUBLIC .)
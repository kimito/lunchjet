cmake_minimum_required(VERSION 3.10)
project("JetBox RC Car")

add_library(jetbox_rc_car STATIC jetbox_rc_car.cpp)
target_link_libraries(jetbox_rc_car jetbox_pca9685)
target_include_directories(jetbox_rc_car PUBLIC .)
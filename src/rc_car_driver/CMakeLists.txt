cmake_minimum_required(VERSION 3.10)
project("LunchJet RC Car")

add_library(lunchjet_rc_car_driver STATIC lunchjet_rc_car_driver.cpp)
target_link_libraries(lunchjet_rc_car_driver lunchjet_pca9685)
target_include_directories(lunchjet_rc_car_driver PUBLIC .)
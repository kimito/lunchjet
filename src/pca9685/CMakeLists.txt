cmake_minimum_required(VERSION 3.10)
project("LunchJet PCA9685")

add_library(lunchjet_pca9685 STATIC lunchjet_pca9685.cpp)
target_link_libraries(lunchjet_pca9685 lunchjet_i2c)
target_include_directories(lunchjet_pca9685 PUBLIC .)
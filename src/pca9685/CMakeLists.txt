cmake_minimum_required(VERSION 3.10)
project("JetBox PCA9685")

add_library(jetbox_pca9685 STATIC jetbox_pca9685.cpp)
target_link_libraries(jetbox_pca9685 jetbox_i2c)
target_include_directories(jetbox_pca9685 PUBLIC .)
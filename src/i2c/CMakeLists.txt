cmake_minimum_required(VERSION 3.10)
project("LunchJet i2c")

add_library(lunchjet_i2c STATIC i2c.cpp)
target_link_libraries(lunchjet_i2c lunchjet_include)
target_include_directories(lunchjet_i2c PUBLIC .)
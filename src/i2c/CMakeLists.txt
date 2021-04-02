cmake_minimum_required(VERSION 3.10)
project("JetBox i2c")

add_library(jetbox_i2c STATIC jetbox_i2c.cpp)
target_link_libraries(jetbox_i2c jetbox_include)
target_include_directories(jetbox_i2c PUBLIC .)
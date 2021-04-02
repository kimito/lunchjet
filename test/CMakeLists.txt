cmake_minimum_required(VERSION 3.10)
project("JetBox Test")

add_executable(i2c_verify_test i2c_verify_test.cpp)
target_link_libraries(i2c_verify_test jetbox_i2c jetbox_include)
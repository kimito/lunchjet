cmake_minimum_required(VERSION 3.10)
project("JetBox include")

add_library(jetbox_include INTERFACE)
target_include_directories(jetbox_include INTERFACE .)
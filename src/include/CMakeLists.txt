cmake_minimum_required(VERSION 3.10)
project("LunchJet include")

add_library(lunchjet_include INTERFACE)
target_include_directories(lunchjet_include INTERFACE .)
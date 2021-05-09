cmake_minimum_required(VERSION 3.10)
project("LunchJet Video Input Device")

find_package( OpenCV REQUIRED )

add_library(lunchjet_video_input_device STATIC video_input_device.cpp)
target_link_libraries(lunchjet_video_input_device lunchjet_include pthread ${OpenCV_LIBS})
target_include_directories(lunchjet_video_input_device PUBLIC .)
target_include_directories(lunchjet_video_input_device PRIVATE ${OpenCV_INCLUDE_DIRS})
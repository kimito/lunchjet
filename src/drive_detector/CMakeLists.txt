cmake_minimum_required(VERSION 3.10)
project("LunchJet drive controller")

find_package( OpenCV REQUIRED )

add_library(lunchjet_drive_detector STATIC drive_detector.cpp)
target_link_libraries(lunchjet_drive_detector lunchjet_include ${OpenCV_LIBS})
target_include_directories(lunchjet_drive_detector PUBLIC .)
target_include_directories(lunchjet_drive_detector PRIVATE ${OpenCV_INCLUDE_DIRS})
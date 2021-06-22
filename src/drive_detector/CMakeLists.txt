cmake_minimum_required(VERSION 3.10)
project("LunchJet drive controller")

find_package( OpenCV REQUIRED )

find_path(TENSORRT_INCLUDE_DIR NvInfer.h)
find_library(TENSORRT_LIB nvinfer
    PATHS /usr/local/cuda/targets/aarch64-linux/lib)

find_path(CUDA_RUNTIME_INCLUDE_DIR cuda_runtime_api.h
    PATHS /usr/local/cuda/targets/aarch64-linux/include/)
find_library(CUDA_RUNTIME_LIB cudart
    PATHS /usr/local/cuda/targets/aarch64-linux/lib)


add_library(lunchjet_drive_detector STATIC drive_detector.cpp)
target_include_directories(lunchjet_drive_detector PUBLIC .)
target_include_directories(lunchjet_drive_detector
    PRIVATE ${OpenCV_INCLUDE_DIRS} ${TENSORRT_INCLUDE_DIR} ${CUDA_RUNTIME_INCLUDE_DIR})
target_link_libraries(lunchjet_drive_detector
    lunchjet_include ${OpenCV_LIBS} ${TENSORRT_LIB} ${CUDA_RUNTIME_LIB})
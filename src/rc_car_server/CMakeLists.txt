cmake_minimum_required(VERSION 3.10)
project("LunchJet RC Car Server")

set(LOG_IMAGE_DIR /var/log/lunchjet/control/images)
set(LOG_ANNOTAION_DIR /var/log/lunchjet/control/annotations)

add_executable(lunchjet_rc_car_server rc_car_server.cpp main.cpp)
target_link_libraries(lunchjet_rc_car_server
 lunchjet_include 
 lunchjet_rc_car_controller 
 lunchjet_rc_car_driver 
 lunchjet_video_input_device)
target_compile_definitions(lunchjet_rc_car_server
 PRIVATE LOG_IMAGE_DIR="${LOG_IMAGE_DIR}"
 PRIVATE LOG_ANNOTAION_DIR="${LOG_ANNOTAION_DIR}")

install(TARGETS lunchjet_rc_car_server DESTINATION /opt/lunchjet/)
install(FILES lunchjet_rc_car.service DESTINATION /usr/lib/systemd/system/)
install(DIRECTORY DESTINATION ${LOG_IMAGE_DIR})
install(DIRECTORY DESTINATION ${LOG_ANNOTAION_DIR})


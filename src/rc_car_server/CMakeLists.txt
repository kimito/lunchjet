cmake_minimum_required(VERSION 3.10)
project("LunchJet RC Car Server")

add_executable(lunchjet_rc_car_server lunchjet_rc_car_server.cpp)
target_link_libraries(lunchjet_rc_car_server lunchjet_include lunchjet_rc_car_controller lunchjet_rc_car_driver)

install(TARGETS lunchjet_rc_car_server DESTINATION "/opt/lunchjet/")
install(FILES lunchjet_rc_car.service DESTINATION /usr/lib/systemd/system/)


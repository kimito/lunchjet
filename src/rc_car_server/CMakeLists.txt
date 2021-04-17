cmake_minimum_required(VERSION 3.10)
project("JetBox RC Car Server")

add_executable(jetbox_rc_car_server jetbox_rc_car_server.cpp)
target_link_libraries(jetbox_rc_car_server jetbox_include jetbox_rc_car_controller jetbox_rc_car_driver)

install(TARGETS jetbox_rc_car_server DESTINATION "/opt/jetbox/")


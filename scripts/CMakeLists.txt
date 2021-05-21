cmake_minimum_required(VERSION 3.10)
project("LunchJet scripts")

file(GLOB files_in_package lunchjet/*.py)

install(FILES upload_training_data.py  DESTINATION /opt/lunchjet/scripts)
install(FILES ${files_in_package}  DESTINATION /opt/lunchjet/scripts/lunchjet)

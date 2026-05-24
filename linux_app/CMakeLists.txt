cmake_minimum_required(VERSION 3.10)

project(MiniCar C)

add_executable(car main.c)
add_executable(sensor_node sensor_node.c)
add_executable(motor_node motor_node.c)
cmake_minimum_required(VERSION 3.26)
project(gpstest)

set(CMAKE_CXX_STANDARD 17)

find_package(serial REQUIRED)

add_executable(gpstest src/main.cpp)

include_directories(mavlink)

target_link_libraries(gpstest PRIVATE ${SERIAL_LIBRARIES})
cmake_minimum_required(VERSION 3.15)
project(i3_switch_notification)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(i3ipc++)

include_directories(${I3IPCpp_INCLUDE_DIRS})
link_directories(${I3IPCpp_LIBRARY_DIRS})
add_executable(i3_switch_notification src/main.cpp)
target_link_libraries(i3_switch_notification ${I3IPCpp_LIBRARIES})
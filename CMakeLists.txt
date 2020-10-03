cmake_minimum_required(VERSION 3.15)
project(i3_switch_notification)
set(CMAKE_CXX_STANDARD 14)
include(FindPkgConfig)
add_subdirectory(i3ipc++)

INCLUDE_DIRECTORIES ("src")

pkg_check_modules (GTK3 REQUIRED gtk+-3.0)

include_directories (
        ${I3IPCpp_INCLUDE_DIRS}
        ${GTK3_INCLUDE_DIRS}
)

link_directories (
        ${I3IPCpp_LIBRARY_DIRS}
        ${GTK3_LIBRARY_DIRS})

set (DIR_SRCS
        src/main.cpp
        src/NotificationMainWindow.cpp
        src/NotificationMainWindow.h
        src/NotificationController.cpp
        src/NotificationController.h
        src/Consts.h)

add_definitions (
        ${GTK3_CFLAGS_OTHER}
)
add_executable(i3_switch_notification ${DIR_SRCS})

target_link_libraries(i3_switch_notification ${I3IPCpp_LIBRARIES} ${GTK3_LDFLAGS})

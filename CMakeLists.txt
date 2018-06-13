cmake_minimum_required(VERSION 2.8.12)

project(playdl-signin-ui-qt)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Widgets WebEngineWidgets REQUIRED)

add_executable(${PROJECT_NAME} main.cpp loginwindow.cpp loginwindow.h materialbusyindicator.cpp materialbusyindicator.h)

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::WebEngineWidgets)

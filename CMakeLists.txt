cmake_minimum_required(VERSION 2.8.12)

project(playdl-signin-ui-qt)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Widgets WebEngineWidgets REQUIRED)

add_library(playdl-signin-ui-qt-lib src/googleloginwindow.cpp include/googleloginwindow.h src/materialbusyindicator.cpp src/materialbusyindicator.h googlesigninui.qrc)
target_link_libraries(playdl-signin-ui-qt-lib PUBLIC Qt6::Core Qt6::Widgets Qt6::WebEngineWidgets)
target_include_directories(playdl-signin-ui-qt-lib PUBLIC include/)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Widgets Qt6::WebEngineWidgets playdl-signin-ui-qt-lib)

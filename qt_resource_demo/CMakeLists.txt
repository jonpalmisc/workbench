cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(qt_resource_demo)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

add_executable(qt_resource_demo main.cpp resources.qrc)
target_compile_features(qt_resource_demo PRIVATE cxx_std_17)
target_link_libraries(qt_resource_demo PUBLIC Qt6::Widgets)

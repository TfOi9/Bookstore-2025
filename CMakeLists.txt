cmake_minimum_required(VERSION 3.16)
project(BookStore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(
    include
    frontend/include
    ${Qt${QT_VERSION_MAJOR}Widgets_INCLUDE_DIRS}
)

add_executable(
    code
    src/account.cpp src/book.cpp src/log.cpp
    src/utils.cpp src/validator.cpp
    src/code.cpp
)

add_executable(cleanup src/cleanup.cpp)

add_executable(
    gui${MOC_SOURCES}
    src/account.cpp src/book.cpp src/log.cpp
    src/utils.cpp src/validator.cpp
    frontend/src/globals.cpp
    frontend/include/book_table.hpp frontend/src/book_table.cpp
    frontend/include/book_control_panel.hpp frontend/src/book_control_panel.cpp 
    frontend/include/top_bar.hpp frontend/src/top_bar.cpp
    frontend/src/main.cpp
)

target_link_libraries(gui PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(gui PROPERTIES AUTOMOC ON)
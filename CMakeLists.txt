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
    frontend/include frontend/include/dialogs frontend/include/panels frontend/include/tables
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
    frontend/include/tables/account_table.hpp frontend/src/tables/account_table.cpp
    frontend/include/tables/book_table.hpp frontend/src/tables/book_table.cpp
    frontend/include/tables/log_table.hpp frontend/src/tables/log_table.cpp
    frontend/include/panels/account_control_panel.hpp frontend/src/panels/account_control_panel.cpp
    frontend/include/panels/book_control_panel.hpp frontend/src/panels/book_control_panel.cpp
    frontend/include/panels/log_control_panel.hpp frontend/src/panels/log_control_panel.cpp
    frontend/include/top_bar.hpp frontend/src/top_bar.cpp
    frontend/include/main_window.hpp frontend/src/main_window.cpp
    frontend/src/main.cpp
)

target_link_libraries(gui PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(gui PROPERTIES AUTOMOC ON)

if(APPLE)
    set_source_files_properties(frontend/icons/BookShelf.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    target_sources(gui PRIVATE frontend/icons/BookShelf.icns)
    set_target_properties(gui PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_ICON_FILE "BookShelf"
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/frontend/Info.plist.in"
        OUTPUT_NAME "BookStore"
    )
    target_sources(gui PRIVATE frontend/icons/icons.qrc)
elseif(WIN32)
    target_sources(gui PRIVATE frontend/icons/app_icon.rc frontend/icons/icons.qrc)
    set_target_properties(gui PROPERTIES WIN32_EXECUTABLE TRUE OUTPUT_NAME "BookStore")
endif()
cmake_minimum_required(VERSION 3.10)
project(BookStore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

include_directories(include)

add_executable(account src/account.cpp src/book.cpp src/account_tester.cpp)

add_executable(book src/account.cpp src/book.cpp src/book_tester.cpp)

add_executable(cleanup src/cleanup.cpp)
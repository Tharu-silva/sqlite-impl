cmake_minimum_required(VERSION 3.15)
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

project(sqlite-impl)

find_package(Boost CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(sqlite-impl src/main.cpp)
target_include_directories(sqlite-impl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(sqlite-impl PRIVATE Boost::boost)
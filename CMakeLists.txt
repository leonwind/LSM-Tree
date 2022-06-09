cmake_minimum_required(VERSION 3.14)

project(KV-Store)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.50 REQUIRED COMPONENTS filesystem)
message(STATUS "Boost version: ${Boost_VERSION}")

add_subdirectory(src)
file(GLOB db_app kv_store.cpp)
add_executable(kv_store ${db_app})
target_link_libraries(kv_store lsm-tree)

add_subdirectory(benchmark)
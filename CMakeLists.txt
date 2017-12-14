cmake_minimum_required(VERSION 3.9)

project(rapidjson_writable)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(PROJECT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(PROJECT_TEST_DIR ${CMAKE_SOURCE_DIR}/test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -g")
set(CMAKE_*_LINKER_FLAGS " -v ${UV_LIB}")

## Deps

include(${CMAKE_CURRENT_SOURCE_DIR}/uv.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/rapidjson.cmake)

## Project

# rapidjson-writable
include_directories(${PROJECT_INCLUDE_DIR})
include_directories(${PROJECT_TEST_DIR})
file(GLOB rapidjson_writable_headers ${PROJECT_INCLUDE_DIR}/*)
file(GLOB rapidjson_writable_test_headers ${PROJECT_TEST_DIR}/*.h)
file(GLOB rapidjson_writable_test_sources ${PROJECT_TEST_DIR}/*.cc)

source_group(include FILES $${rapidjson_writable_headers})
source_group(test FILES ${rapidjson_writable_test_sources} ${rapidjson_writable_test_headers})

add_executable(test_dump
  ${rapidjson_writable_headers}
  ${PROJECT_TEST_DIR}/test_dump.cc ${rapidjson_writable_test_headers}
  ${rapidjson_headers} ${uv_headers} ${uv_sources}
)

add_executable(test_minify
  ${rapidjson_writable_headers}
  ${PROJECT_TEST_DIR}/test_minify.cc ${rapidjson_writable_test_headers}
  ${rapidjson_headers} ${uv_headers} ${uv_sources}
)

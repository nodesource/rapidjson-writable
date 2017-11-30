cmake_minimum_required(VERSION 3.9)

project(rapidjson_writable)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(PROJECT_TEST_DIR ${CMAKE_SOURCE_DIR}/test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -g")
set(CMAKE_*_LINKER_FLAGS " -v ${UV_LIB}")

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_TEST_DIR})
include_directories(${PROJECT_SOURCE_DIR}/../deps/uv/include)
include_directories("${PROJECT_SOURCE_DIR}/../deps/rapidjson/include")

file(GLOB rapidjson_writable_headers ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB rapidjson_writable_sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB rapidjson_writable_test_headers ${PROJECT_TEST_DIR}/*.h)
file(GLOB rapidjson_writable_test_sources ${PROJECT_TEST_DIR}/*.cc)
file(GLOB uv_headers ${PROJECT_SOURCE_DIR}/../deps/uv/include/*.h)
file(GLOB uv_sources ${PROJECT_SOURCE_DIR}/../deps/uv/*.cc)
file(GLOB uv_unix_sources ${PROJECT_SOURCE_DIR}/../deps/uv/unix/*.cc)
file(GLOB rapidjson_headers ${PROJECT_SOURCE_DIR}/../deps/rapidjson/include/rapidjson/*.h)
file(GLOB rapidjson_error_headers ${PROJECT_SOURCE_DIR}/../deps/rapidjson/include/rapidjson/error/*.h)
file(GLOB rapidjson_internal_headers ${PROJECT_SOURCE_DIR}/../deps/rapidjson/include/rapidjson/internal/*.h)
file(GLOB rapidjson_msinttypes_headers ${PROJECT_SOURCE_DIR}/../deps/rapidjson/include/rapidjson/msinttypes/*.h)

source_group(src FILES ${rapidjson_writable_sources} ${rapidjson_writable_headers})
source_group(test FILES ${rapidjson_writable_test_sources} ${rapidjson_writable_test_headers})
source_group(uv FILES ${uv_sources} ${uv_unix_sources} ${uv_headers})
source_group(rapidjson FILES ${rapidjson_headers})
source_group(rapidjson/error FILES ${rapidjson_error_headers})
source_group(rapidjson/internal FILES ${rapidjson_internal_headers})
source_group(rapidjson/msinttypes FILES ${rapidjson_msinttypes_headers})

add_executable(test_dump
  ${rapidjson_writable_sources} ${rapidjson_writable_headers}
  ${PROJECT_TEST_DIR}/test_dump.cc ${rapidjson_writable_test_headers}
  ${uv_sources} ${uv_unix_sources} ${uv_headers}
  ${rapidjson_headers} ${rapidjson_error_headers} ${rapidjson_internal_headers} ${rapidjson_msinttypes_headers}
)

add_executable(test_minify
  ${rapidjson_writable_sources} ${rapidjson_writable_headers}
  ${PROJECT_TEST_DIR}/test_minify.cc ${rapidjson_writable_test_headers}
  ${uv_sources} ${uv_unix_sources} ${uv_headers}
  ${rapidjson_headers} ${rapidjson_error_headers} ${rapidjson_internal_headers} ${rapidjson_msinttypes_headers}
)

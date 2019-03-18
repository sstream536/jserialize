cmake_minimum_required(VERSION 2.8)

project(example)

SET(CMAKE_C_COMPILER g++)
SET(CMAK_CXX_COMPILER g++)
add_compile_options(-std=c++11)

set(SOURCE_FILES src/example.cpp
  include/jserialize.hpp)

 set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
include_directories(
${CMAKE_CURRENT_SOURCE_DIR}/include
${CMAKE_CURRENT_SOURCE_DIR}/include/rapidjson/include
)

add_executable(example
${SOURCE_FILES}
)


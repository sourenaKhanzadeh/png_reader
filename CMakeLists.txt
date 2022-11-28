cmake_minimum_required(VERSION 3.0)

project(ImageReader
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "ImageReader"
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${INCLUDE_DIR})

add_library(Reader
    ${INCLUDE_DIR}/png_reader/png_reader.h
    ${SOURCE_DIR}/png_reader.cpp
)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} Reader)
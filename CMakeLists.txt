cmake_minimum_required(VERSION 3.20)
project(epoch)

set(CMAKE_CXX_STANDARD 23)

include_directories(${PROJECT_SOURCE_DIR}/)
include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(${PROJECT_SOURCE_DIR}/v8 SRC_V8)
aux_source_directory(${PROJECT_SOURCE_DIR}/init SRC_INIT)

add_executable(EPOCH ${SRC_V8} ${SRC_INIT})

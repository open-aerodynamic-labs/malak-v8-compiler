cmake_minimum_required(VERSION 3.20)
project(volcanos C)

set(CMAKE_C_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR})

aux_source_directory(${PROJECT_SOURCE_DIR}/v8 v8_dir)
aux_source_directory(${PROJECT_SOURCE_DIR}/main main_dir)
aux_source_directory(${PROJECT_SOURCE_DIR}/tool tool_dir)

add_executable(volcanos ${v8_dir} ${main_dir} ${tool_dir})

cmake_minimum_required(VERSION 3.20)
project(v8 C)

set(CMAKE_C_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR} include)

aux_source_directory(${PROJECT_SOURCE_DIR}/v8 v8_dir)
aux_source_directory(${PROJECT_SOURCE_DIR}/vklib vklib_dir)
aux_source_directory(${PROJECT_SOURCE_DIR}/vklib/io vklib_io_dir)
aux_source_directory(${PROJECT_SOURCE_DIR}/main main_dir)
aux_source_directory("" DIR_END)

add_executable(${PROJECT_NAME}
        ${v8_dir}
        ${main_dir}
        ${vklib_dir}
        ${vklib_io_dir}
        ${DIR_END}
)

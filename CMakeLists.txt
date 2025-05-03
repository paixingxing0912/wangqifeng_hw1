cmake_minimum_required (VERSION 3.10)

project (main C)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

aux_source_directory (src SRC_LIST)

include_directories (inc)

add_executable (main ${SRC_LIST})

set(CMAKE_C_STANDARD 99)   # 使用 C99
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

target_compile_features(main PRIVATE c_std_99)
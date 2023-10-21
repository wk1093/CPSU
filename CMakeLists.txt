cmake_minimum_required(VERSION 3.26)
project(cpsu C)

set(CMAKE_C_STANDARD 17)

add_executable(cpsu src/main.c)

target_include_directories(cpsu PUBLIC dist)

# wall and werror
target_compile_options(cpsu PUBLIC -Wall -Wextra -Werror)

# recurse into subdirectories of ${CMAKE_CURRENT_SOURCE_DIR}/src/cpsu/ for DEPENDS
file(GLOB_RECURSE cpsu_srcs ${CMAKE_CURRENT_SOURCE_DIR}/src/cpsu/*.c)
file(GLOB_RECURSE cpsu_hdrs ${CMAKE_CURRENT_SOURCE_DIR}/src/cpsu/*.h)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/dist/cpsu/cpsu.h
        COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/dist/gen.py
            -r ${CMAKE_CURRENT_SOURCE_DIR}/src/cpsu
            -o ${CMAKE_CURRENT_SOURCE_DIR}/dist/cpsu/cpsu.h
            ${CMAKE_CURRENT_SOURCE_DIR}/src/cpsu/cpsu.h
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ${cpsu_srcs} ${cpsu_hdrs}

        COMMENT "Generating cpsu.h"
        PRE_BUILD VERBATIM
)

add_custom_target(cpsu_gen DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/dist/cpsu/cpsu.h)

add_dependencies(cpsu cpsu_gen)

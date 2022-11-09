cmake_minimum_required(VERSION 3.23)
project(kalkumulator)

set(CMAKE_CXX_STANDARD 23)

set(TARGET_NAME kalkumulator)

add_executable(${TARGET_NAME}
        main.cpp
        tokens.hpp
        types.hpp
        scanner.hpp
        error.hpp
        utils.hpp
        utils.cpp
        error.cpp
        parser.hpp
        expressions.hpp
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${TARGET_NAME} PUBLIC -Wall -Wextra -Werror -Wconversion -pedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(${TARGET_NAME} PUBLIC -Wall -Wextra -Werror -Wconversion -pedantic)
    else ()
        target_compile_options(${TARGET_NAME} PUBLIC -Wall -Wextra -Werror -Wconversion -pedantic -fsanitize=address,undefined)
        target_link_options(${TARGET_NAME} PUBLIC -fsanitize=address,undefined)
    endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(${TARGET_NAME} PUBLIC /W4 /WX /permissive-)
    else ()
        target_compile_options(${TARGET_NAME} PUBLIC /W4 /WX /permissive- /fsanitize=address)
    endif()
endif()

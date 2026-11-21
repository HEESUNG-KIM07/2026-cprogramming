cmake_minimum_required(VERSION 3.10)
project(acid_rain C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(acid_rain
    main.c
    acid_rain.c
)

if(MSVC)
    target_compile_options(acid_rain PRIVATE /utf-8 /W3)
else()
    target_compile_options(acid_rain PRIVATE -Wall -Wextra -finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()
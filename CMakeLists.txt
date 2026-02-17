cmake_minimum_required(VERSION 3.16)
project(rts C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

add_executable(rts
    src/main.c
)

target_link_libraries(rts
    ${CMAKE_SOURCE_DIR}/lib/libraylib.a
    m dl pthread GL rt X11
)

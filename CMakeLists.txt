cmake_minimum_required(VERSION 3.28)
project(microb C)
find_package(PkgConfig)

set(CMAKE_C_STANDARD 11)

add_library(microb SHARED
        src/window.c
        include/microb/window.h
        src/utils.c
        src/utils.h
        src/luart.c
        src/luart.h)
target_include_directories(microb PRIVATE include)
pkg_check_modules(LUA REQUIRED lua5.4)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(HARFBUZZ REQUIRED harfbuzz)
pkg_check_modules(FREETYPE2 REQUIRED freetype2)

target_include_directories(microb PUBLIC
        ${LUA_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${HARFBUZZ_INCLUDE_DIRS}
        ${FREETYPE2_INCLUDE_DIRS})
target_link_libraries(microb
        ${LUA_LIBRARIES} ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${HARFBUZZ_LIBRARIES} ${FREETYPE2_LIBRARIES})
target_link_libraries(microb -fsanitize=address -fsanitize=undefined)
target_compile_options(microb PUBLIC
        ${LUA_CFLAGS_OTHER} ${SDL2_CFLAGS_OTHER} ${SDL2_TTF_CFLAGS_OTHER} ${HARFBUZZ_CFLAGS_OTHER}
        ${FREETYPE2_CFLAGS_OTHER})

macro(configure_test_case target)
    target_include_directories(${target} PRIVATE include)
    target_link_libraries(${target} microb)
endmacro()

add_executable(test-hello-world
        tests/non-automated/hello-world.c)
configure_test_case(test-hello-world)
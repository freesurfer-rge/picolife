cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include($ENV{PICO_SDK_PATH}/tools/CMakeLists.txt)

project(picolife C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Warning options for the compiler
string(
  APPEND _warning_opts
  "$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall;-Wextra;-Weffc++;-Werror;>"
  "$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Wthread-safety;-Wpedantic;>"
  "$<$<CXX_COMPILER_ID:GNU>:-pedantic;-pedantic-errors;>"
  )

# add_compile_options("${_warning_opts}")

pico_sdk_init()

add_executable(${PROJECT_NAME}
    main.cpp
    ledimage.cpp
    colourvector.cpp
)


pico_add_extra_outputs(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_pio
    pico_multicore
    sparselife
    leddriver
)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

add_subdirectory(leddriver)
add_subdirectory(sparselife)
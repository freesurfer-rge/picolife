add_library(leddriver)

target_include_directories(leddriver
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)


target_link_libraries(leddriver
  PUBLIC pico_stdlib
  PUBLIC hardware_pio)

add_subdirectory(src)
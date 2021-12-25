add_executable(TestCard
    ${CMAKE_CURRENT_SOURCE_DIR}/testCard.cpp
)
pico_add_extra_outputs(TestCard)
target_link_libraries(TestCard
    PUBLIC pico_stdlib
    PUBLIC leddriver
)

pico_enable_stdio_usb(TestCard 1)
pico_enable_stdio_uart(TestCard 0)
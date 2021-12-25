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

# ==========================================

add_executable(SquareDiagonal
    ${CMAKE_CURRENT_SOURCE_DIR}/squareDiagonal.cpp
)
pico_add_extra_outputs(SquareDiagonal)
target_link_libraries(SquareDiagonal
    PUBLIC pico_stdlib
    PUBLIC leddriver
)

pico_enable_stdio_usb(SquareDiagonal 1)
pico_enable_stdio_uart(SquareDiagonal 0)
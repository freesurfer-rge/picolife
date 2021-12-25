set(srcs)

pico_generate_pio_header(leddriver
    ${CMAKE_CURRENT_LIST_DIR}/led_line.pio
)

list(APPEND srcs ${CMAKE_CURRENT_SOURCE_DIR}/pioCommunicator.cpp)
list(APPEND srcs ${CMAKE_CURRENT_SOURCE_DIR}/ledarray.cpp)
list(APPEND srcs ${CMAKE_CURRENT_SOURCE_DIR}/ledimage.cpp)
list(APPEND srcs ${CMAKE_CURRENT_SOURCE_DIR}/utility.cpp)

target_sources(leddriver PRIVATE ${srcs})
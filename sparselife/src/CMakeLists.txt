set(srcs)

list(APPEND srcs ${CMAKE_CURRENT_SOURCE_DIR}/cellpattern.cpp)
list(APPEND srcs ${CMAKE_CURRENT_SOURCE_DIR}/sparselife.cpp)

target_sources(sparselife PRIVATE ${srcs})
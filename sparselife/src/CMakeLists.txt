set(srcs)

list(APPEND srcs ${CMAKE_CURRENT_SOURCE_DIR}/cellpattern.cpp)

target_sources(sparselife PRIVATE ${srcs})
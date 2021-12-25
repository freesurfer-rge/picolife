set(srcs)

list(APPEND srcs ${CMAKE_CURRENT_SOURCE_DIR}/pioCommunicator.cpp)

target_sources(leddriver PRIVATE ${srcs})
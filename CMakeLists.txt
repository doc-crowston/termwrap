cmake_minimum_required (VERSION 3.8.0 FATAL_ERROR)

project (TermWrap)



set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -ggdb -Wfatal-errors")

find_library(TERMBOX termbox /opt/termbox/lib)

add_library(cell_style cell_style.cpp)
add_library(driver driver.cpp)
add_library(textbox textbox.cpp)
target_link_libraries(driver cell_style)
target_link_libraries(driver ${TERMBOX})
target_link_libraries(cell_style ${TERMBOX})
target_link_libraries(textbox driver)

add_executable(demo demo.cpp)
target_link_libraries(demo driver)
target_link_libraries(demo textbox)


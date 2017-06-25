cmake_minimum_required (VERSION 3.4.0 FATAL_ERROR)

project (TermWrap)



set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_library(TERMBOX termbox /opt/termbox/lib)

add_library(cell_style cell_style.cpp)
add_library(driver driver.cpp)
target_link_libraries(driver cell_style)
target_link_libraries(driver ${TERMBOX})
target_link_libraries(cell_style ${TERMBOX})


add_executable(demo demo.cpp)
target_link_libraries(demo driver)


cmake_minimum_required(VERSION 3.1)
project(CGALProject)

find_package(CGAL REQUIRED)
include(${CGAL_USE_FILE})

add_executable(cgal_example example.cpp)
target_link_libraries(cgal_example ${CGAL_LIBRARIES})
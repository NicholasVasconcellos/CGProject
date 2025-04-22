cmake_minimum_required(VERSION 3.1)
project(CGALProject)

# 1) Use CGAL with Qt5
find_package(CGAL COMPONENTS Qt5 REQUIRED)
include(${CGAL_USE_FILE})

# 2) Define CGAL_USE_BASIC_VIEWER
add_definitions(-DCGAL_USE_BASIC_VIEWER)

# 3) Create executable and link with CGAL and CGAL_Qt5
add_executable(cgal_example main.cpp 2dCluster.cpp)
target_link_libraries(cgal_example PUBLIC CGAL::CGAL CGAL::CGAL_Qt5)

# # 4) Create Test Executable
# add_executable(myTest test.cpp)
# target_link_libraries(myTest PUBLIC CGAL::CGAL CGAL::CGAL_Qt5)
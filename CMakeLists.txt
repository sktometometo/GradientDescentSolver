cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(report1)

set(CMAKE_CXX_FLAGS "-O2 -std=c++11 -Wall")

find_package(Boost COMPONENTS program_options REQUIRED)
find_package(Eigen3 REQUIRED)

include_directories(
    include
    lib/matplotlib-cpp
    ${Boost_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIR}
)

add_executable(01_nonconvex
    src/01_nonconvex.cpp
)
target_link_libraries(01_nonconvex
    ${Boost_LIBRARIES}
)

add_executable(02_convex
    src/02_convex.cpp
)
target_link_libraries(02_convex
    ${Boost_LIBRARIES}
)

add_executable(03_strongconvex
    src/03_strongconvex.cpp
)
target_link_libraries(03_strongconvex
    ${Boost_LIBRARIES}
)

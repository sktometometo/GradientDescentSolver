cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(report1)

find_package(Eigen3 REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)

include_directories(
    include
    ${EIGEN3_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)

add_executable(02_convex src/02_convex.cpp)
target_link_libraries(02_convex ${Boost_LIBRARIES})

add_executable(04_convex_with_acceleration src/04_convex_with_acceleration.cpp)
target_link_libraries(04_convex_with_acceleration ${Boost_LIBRARIES})

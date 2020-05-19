cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(report1)

find_package(Eigen3 REQUIRED)
find_package(Boost REQUIRED)

include_directories(
    include
    ${EIGEN3_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)

add_executable(test01 src/test01.cpp)
target_link_libraries(test01 ${Boost_LIBRARIES})

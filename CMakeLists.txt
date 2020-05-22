cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(report1)

find_package(Boost COMPONENTS program_options REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(PythonLibs REQUIRED)
find_package(PythonInterp REQUIRED)

include_directories(
    include
    lib/matplotlib-cpp
    ${Boost_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIR}
    ${PYTHON_INCLUDE_DIRS}
)

add_executable(sample
    src/sample.cpp
)
target_link_libraries(sample
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARIES}
)

add_executable(02_convex
    src/02_convex.cpp
)
target_link_libraries(02_convex
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARIES}
)

add_executable(03_strongconvex src/03_strongconvex.cpp)
target_link_libraries(03_strongconvex ${Boost_LIBRARIES})

add_executable(04_convex_with_acceleration src/04_convex_with_acceleration.cpp)
target_link_libraries(04_convex_with_acceleration ${Boost_LIBRARIES})

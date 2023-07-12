cmake_minimum_required(VERSION 3.12)
project(Simulator)

find_package(BLAS REQUIRED)

add_subdirectory(src/utils)

# Add the source files to the project
set(SOURCES
    src/simulator.cpp
)

# Add an executable target and link the source files
add_executable(simulator ${SOURCES})
target_link_libraries(simulator BLAS::BLAS utils)
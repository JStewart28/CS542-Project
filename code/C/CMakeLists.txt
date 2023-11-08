cmake_minimum_required(VERSION 3.10)

project(MPI_Project)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_STANDARD 99)

find_package(MPI REQUIRED)
# add this line only when you are using openmpi which has a different c++ bindings
add_definitions(-DOMPI_SKIP_MPICXX)

# Use imported targets would make things much eazier. Thanks Levi for pointing it out.
include_directories(${CMAKE_SOURCE_DIR})

add_executable(all_to_all src/all_to_all.cpp)
target_link_libraries(all_to_all PUBLIC MPI::MPI_CXX m)

add_executable(count_primes src/count_primes.cpp)
target_link_libraries(count_primes PUBLIC MPI::MPI_CXX m)

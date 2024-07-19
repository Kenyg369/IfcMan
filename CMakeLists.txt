cmake_minimum_required(VERSION 3.28)
project(IfcMan)

set(CMAKE_CXX_STANDARD 17)

# Include the Manifold headers
include_directories(/usr/local/include/manifold)
include_directories(/usr/local/include/eigen3)
include_directories(/home/ken/Project/Man/boost)

link_directories(/usr/local/lib)

add_executable(IfcMan main.cpp
#        util.cpp
#        util.h
        taxonomy.h
        taxonomy.cpp)

# Link against the Manifold library(only link the non header-only library, e.g: eigen)
target_link_libraries(IfcMan PRIVATE manifold)



## Find the glm package (If CMake configuration file is provided)
#find_package(glm CONFIG REQUIRED)
#target_link_libraries(IfcMan PRIVATE manifold glm::glm)

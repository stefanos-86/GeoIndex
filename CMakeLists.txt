cmake_minimum_required(VERSION 2.8)
project(geometryindex)

#
# Stuff for tests. 
# Check comment on top of main.cpp for gtest install instructions.
# Probably not portable to Windows.
#
find_package(GTest REQUIRED)
find_package (Threads) # This is for the pthread library - "apt get" should give it if it is not already there.
enable_testing() # For some absurd reasons this is how you tell this thing to link pthread in gtest (http://stackoverflow.com/questions/31580549/cmake-does-not-consider-pthread)

include_directories(
  ${GTEST_INCLUDE_DIRS}
)

link_directories(
   ${GTEST_LIBRARIES}
)


# 
# C++ 11 for practicality.
# -O3 to optimize.
# -march=native to optimize even more (but the binary can not be used on another machine, or with valgrind).
# -Wall to spot issues.
# -fopenmp because (with gomp below) gives parallel algorithms before having c++17.
add_definitions(-std=c++11 -O3 -Wall  -fopenmp -march=native)

set (FILES_TO_COMPILE
      BasicGeometryTest.cpp
      NoIndexTest.cpp
#     ReferenceSolution.cpp
#     ReferenceSolutionTest.cpp
#     GeometryIndex.cpp
#     GeometryIndexTest.cpp
#     CubeIndex.cpp
#     CubeIndexTest.cpp
#     PerfTest.cpp
     main.cpp
)

# Normal version of the thing built for maximum speed.
add_executable(geometryindex ${FILES_TO_COMPILE})
target_link_libraries(geometryindex ${GTEST_BOTH_LIBRARIES} pthread gomp)

# This version has safety checks (overflow, exceptions on error...) inside.
# The properly paranoid person should use this.
# TODO: check if there is any performance effect.
add_executable(geometryindex_checked ${FILES_TO_COMPILE})
target_link_libraries(geometryindex_checked ${GTEST_BOTH_LIBRARIES} pthread gomp)
target_compile_definitions(geometryindex_checked PRIVATE GEO_INDEX_SAFETY_CHECKS)

# TODO: Parallel/normal targets (with/without gomp).


install(TARGETS geometryindex RUNTIME DESTINATION bin)
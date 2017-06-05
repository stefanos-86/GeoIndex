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
# -fopenmp because (with gomp below) gives parallel algorithms before having c++17 (not used)
add_definitions(-std=c++11 -O3 -Wall -march=native)

set (FILES_TO_COMPILE
     BasicGeometryTest.cpp
     DomainAssertions.cpp
     NoIndexTest.cpp
     CommonTest.cpp
     AabbIndexTest.cpp
     CubeIndexTest.cpp
     NearestNeighborsTest.cpp
     main.cpp
)

set (LIBRARIES_I_PILFERED 
    ${GTEST_BOTH_LIBRARIES}
    pthread
    # gomp Only for parallel sort - not used now.
)

# Normal version of the thing built for maximum speed.
add_executable(geometryindex ${FILES_TO_COMPILE})
target_link_libraries(geometryindex ${LIBRARIES_I_PILFERED})

# This version has safety checks (overflow, exceptions on error...) inside.
# The properly paranoid person should use this.
add_executable(geometryindex_checked ${FILES_TO_COMPILE})
target_link_libraries(geometryindex_checked ${LIBRARIES_I_PILFERED})
target_compile_definitions(geometryindex_checked PRIVATE GEO_INDEX_SAFETY_CHECKS)

# Builds for performance tests. Those are slow to execute, so they get their own targets.
set (PERFORMANCE_FILES
    PerfTest.cpp
    main.cpp
)
add_executable(perfTests ${PERFORMANCE_FILES})
target_link_libraries(perfTests ${LIBRARIES_I_PILFERED})

add_executable(perfTests_checked ${PERFORMANCE_FILES})
target_link_libraries(perfTests_checked ${LIBRARIES_I_PILFERED})
target_compile_definitions(perfTests_checked PRIVATE GEO_INDEX_SAFETY_CHECKS)


install(TARGETS geometryindex RUNTIME DESTINATION bin)
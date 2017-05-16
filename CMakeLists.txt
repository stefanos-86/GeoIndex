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
# -march=native to optimize even more (but the binary can not be used on another machine).
# -Wall to spot issues.
add_definitions(-std=c++11 -march=native -O3 -Wall)

set (FILES_TO_COMPILE
     Support.cpp
     SupportTest.cpp
     ReferenceSolution.cpp
     ReferenceSolutionTest.cpp
     GeometryIndex.cpp
     GeometryIndexTest.cpp
     main.cpp
)

add_executable(geometryindex ${FILES_TO_COMPILE})
target_link_libraries(geometryindex ${GTEST_BOTH_LIBRARIES} pthread)


install(TARGETS geometryindex RUNTIME DESTINATION bin)
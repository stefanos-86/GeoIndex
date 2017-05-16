/* To get gtest:
 *  sudo apt-get install gtest-dev
 * Then go in the folder (/usr/src/gtest) and
 *  sudo cmake -DBUILD_SHARED_LIBS=ON CMakeLists.txt
 *  sudo make
 * 
 * ...there should be better instructions on Google.
 */

#include "gtest/gtest.h"

int main(int argc, char** argv) {
    
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
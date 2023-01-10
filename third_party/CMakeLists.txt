cmake_minimum_required(VERSION 3.19.0)

if(CONCURRENT_RED_BLACK_TREE_BUILD_TESTS)
    # GoogleTest, https://github.com/google/googletest
    CPMAddPackage("gh:google/googletest#release-1.12.1")
endif(CONCURRENT_RED_BLACK_TREE_BUILD_TESTS)
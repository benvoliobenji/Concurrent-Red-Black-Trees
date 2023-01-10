cmake_minimum_required(VERSION 3.23)

project(CONCURRENT_RED_BLACK_TREE 
        VERSION 1.0.0
        DESCRIPTION "A thread-safe red-black tree implementation."
        LANGUAGES CXX)

# List user/build options
option(CONCURRENT_RED_BLACK_TREE_BUILD_DOCS "Build Documentation" OFF)
option(CONCURRENT_RED_BLACK_TREE_BUILD_TESTS "Build Test" OFF)
option(CONCURRENT_RED_BLACK_TREE_BUILD_DEPENDENCIES "Build Dependencies" OFF)

# Add helper cmake scripts
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(ClangFormat)
include(ClangTidy)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there.")
endif()

# Define our C++ compiler standards to use
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Have external code downlaod into CPM_SOURCE_CACHE or build/cpm_cache folder
if(CUSTOM_CPM_SOURCE_CACHE)
    set(CPM_SOURCE_CACHE ${CUSTOM_CPM_SOURCE_CACHE} CACHE STRING "CPM Cache Location" FORCE)
else()
    set(CPM_SOURCE_CACHE ${CMAKE_SOURCE_DIR}/build/cpm_cache CACHE STRING "CPM Cache Location" FORCE)
endif()
include(CPM)

include(add_dependencies)

# Add third party/vendor code
add_subdirectory(third_party)

# For threading
find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(CONCURRENT_RED_BLACK_TREE PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(CONCURRENT_RED_BLACK_TREE "${CMAKE_THREAD_LIBS_INIT}")
endif()

set(CONCURRENT_RED_BLACK_TREE_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(CONCURRENT_RED_BLACK_TREE_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(CONCURRENT_RED_BLACK_TREE_SRC ${CMAKE_CURRENT_LIST_DIR}/src)

if(CONCURRENT_RED_BLACK_TREE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif(CONCURRENT_RED_BLACK_TREE_BUILD_TESTS)

if(CONCURRENT_RED_BLACK_TREE_BUILD_DOCS)
    include(AddCMakeTools)
    set(BUILD_DOCUMENTATION ON)
    include(${cmake-scripts_SOURCE_DIR}/doxygen.cmake)

    build_docs(DOXYFILE_PATH ${CMAKE_SOURCE_DIR}/docs/Doxyfile OUTPUT_DIR ${CMAKE_SOURCE_DIR}/docs/build)
endif(CONCURRENT_RED_BLACK_TREE_BUILD_DOCS)

if(CONCURRENT_RED_BLACK_TREE_BUILD_DEPENDENCIES)
    include(AddCMakeTools)
    # generate a dep-graph-ProjectName.pdf file showing the project library dependencies
    set(BUILD_DEP_GRAPH ON)
    include(${cmake-scripts_SOURCE_DIR}/dependency-graph.cmake)

    gen_dep_graph(pdf)
endif(CONCURRENT_RED_BLACK_TREE_BUILD_DEPENDENCIES)

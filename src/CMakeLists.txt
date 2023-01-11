cmake_minimum_required(VERSION 3.23)

add_library(${CMAKE_PROJECT_NAME})

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    main.cpp
    Node.cpp
    Parser.cpp
    RBTree.cpp
    RBTreeRun.cpp
    trim.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    ${CONCURRENT_RED_BLACK_TREE_INCLUDE}
)

target_clangformat_setup(${CMAKE_PROJECT_NAME})
setup_clang_tidy()

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
"$<$<CXX_COMPILER_ID:GNU>:-Werror>"
# "$<$<CXX_COMPILER_ID:GNU>:-Wno-unused-parameter>"
# "$<$<CXX_COMPILER_ID:GNU>:-Wno-narrowing>"
# "$<$<CXX_COMPILER_ID:GNU>:-Wno-conversion>"
# "$<$<CXX_COMPILER_ID:GNU>:-Wno-unused-function>"
)
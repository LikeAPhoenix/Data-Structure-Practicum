cmake_minimum_required(VERSION 3.0.0)
project(MyHuffman VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(MyHuffman main.cpp MyHuffman.cpp MyHuffman.h)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

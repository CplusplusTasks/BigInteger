cmake_minimum_required(VERSION 2.8)

project(BIGINT)

include_directories(${BIGINT_SOURCE_DIR} headers)

set(SOURCE_EXE big_integer_testing.cpp
               my_vector.cpp
               big_integer.cpp
               gtest/gtest-all.cc
               gtest/gtest_main.cc)

add_executable(main ${SOURCE_EXE})

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic -O3")
  
endif()

target_link_libraries(main -lgmp -lgmpxx -lpthread)


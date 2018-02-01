cmake_minimum_required(VERSION 3.5)

project(CsvCpp)
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall")

set(LIB_NAME "csvcpp")

set(DEP_DIR "${CMAKE_CURRENT_LIST_DIR}/dep")

set(SRC "src/CSV.cpp")
include_directories("src")
add_library(${LIB_NAME} ${SRC})

if(${BUILD_TEST})
    add_subdirectory("dep")
    include_directories(${CATCH2_INCLUDE_DIR})
    set(TEST_NAME "TestCSVCpp")
    set(TEST_SRC "test/TestCSV.cpp")
    add_executable(${TEST_NAME} ${TEST_SRC})
    target_link_libraries(${TEST_NAME} ${LIB_NAME})
 endif(${BUILD_TEST})

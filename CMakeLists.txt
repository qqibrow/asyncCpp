cmake_minimum_required(VERSION 2.8)

# setup variables that across all the project. Change with 'cmake -Dmyvarname=ON'
set(PROJECT_NAME ProjectTemplate)
set(LIBRARY_NAME TemplateExercise)

# Options. Turn on with 'cmake -Dmyvarname=ON'.
#option(test "Build all tests." OFF) # Makes boolean 'test' available.
if(APPLE)
    # AppleClang doesn't support some latest c++11 feature very well, such as variadic templates. switch to g++
    set(CMAKE_CXX_COMPILER g++)
    set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11 -stdlib=libc++")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "-g -Wall -pthread -std=c++11")
endif()

project(${PROJECT_NAME})

# setup c++ compile options flag
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# setup library name
add_subdirectory(src)

# This adds another subdirectory, which has 'project(gtest)'.
add_subdirectory(dep/gtest-1.7.0)
enable_testing()

# Include the gtest library. gtest_SOURCE_DIR is available due to
# 'project(gtest)' above.
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# header files directory and boost directory
include_directories(./include)

##############
# Unit Tests
##############
AUX_SOURCE_DIRECTORY(tests TEST_LIST)
add_executable(runUnitTests ${TEST_LIST})
# Standard linking to gtest stuff.
target_link_libraries(runUnitTests gtest gtest_main)
# Extra linking for the project.
target_link_libraries(runUnitTests ${library_name})
add_test(mytest runUnitTests)

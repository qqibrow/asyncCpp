cmake_minimum_required(VERSION 2.8)

INCLUDE_DIRECTORIES(../include)
AUX_SOURCE_DIRECTORY(. SRC_LIST)

add_library(${LIBRARY_NAME} SHARED ${SRC_LIST})


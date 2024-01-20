cmake_minimum_required(VERSION 3.22)
project(physfsrwops C)

add_library(
  physfsrwops STATIC
  "${CMAKE_CURRENT_SOURCE_DIR}/src/physfsrwops.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/physfsrwops.h"
)

target_include_directories(
  physfsrwops
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
  PRIVATE "$<TARGET_PROPERTY:physfs-static,INTERFACE_INCLUDE_DIRECTORIES>"
  PRIVATE "$<TARGET_PROPERTY:SDL2-static,INTERFACE_INCLUDE_DIRECTORIES>"
)

cmake_minimum_required(VERSION 3.0)

project(
  T
  VERSION 0.1
  DESCRIPTION "TST"
  LANGUAGES CXX)

add_compile_options(-mwindows -std=c++20)
#SET(LINKER_FLAGS "-lgdiplus")
#add_compile_options(-std=c++20)

add_executable (a a.cpp)

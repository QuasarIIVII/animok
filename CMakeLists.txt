cmake_minimum_required(VERSION 3.0)

project(
  T
  VERSION 0.1
  DESCRIPTION "TST"
  LANGUAGES CXX)

add_compile_options(-mwindows)

add_executable (a a.cpp)

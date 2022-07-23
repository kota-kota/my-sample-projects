cmake_minimum_required(VERSION 3.10)
project(my-sample-projects)

if(MSVC)
  set(CMAKE_SUPPRESS_REGENERATION true)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

if(ANDROID)
  find_library(log-lib log)
endif()

add_subdirectory(standard)
add_subdirectory(thread)
add_subdirectory(logger)

cmake_minimum_required(VERSION 3.10)
project(standard)

add_subdirectory(tests)

add_library(${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)
target_compile_options(${PROJECT_NAME}
  PUBLIC
    $<$<CXX_COMPILER_ID:MSVC>: /W4>
    $<$<CXX_COMPILER_ID:GNU>: -Wall -Wextra -Werror>
    $<$<CXX_COMPILER_ID:Clang>: -Wall -Wextra -Werror>
)
target_sources(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/function.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/function.hpp
)
target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(MSVC)
  set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "standard")
endif()

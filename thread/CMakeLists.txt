cmake_minimum_required(VERSION 3.10)
project(thread)

find_package(Threads REQUIRED)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Timer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Thread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Timer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Thread.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/LockedQueue.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Queue.hpp
)
target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(${PROJECT_NAME}
  PUBLIC
    Threads::Threads
)

if(MSVC)
  set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "thread")
endif()

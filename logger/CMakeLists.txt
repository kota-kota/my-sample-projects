cmake_minimum_required(VERSION 3.10)
project(logger)

set(LOG_WRITER_KIND "terminal" CACHE STRING "Select log writer")
set_property(CACHE LOG_WRITER_KIND PROPERTY STRINGS terminal file)

add_subdirectory(writer)
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
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Logger.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LoggerImpl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/LoggerImpl.hpp
)
target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    log_writer
)

if(MSVC)
  set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "logger")
endif()

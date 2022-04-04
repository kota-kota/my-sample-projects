cmake_minimum_required(VERSION 3.10)
project(test_standard)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sample_rvalue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_function.cpp
)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    standard
)

if(MSVC)
  set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "standard")
endif()

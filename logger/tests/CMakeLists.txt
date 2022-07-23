cmake_minimum_required(VERSION 3.10)
project(test_logger)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_logger.cpp
)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    logger
)

if(MSVC)
  set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "logger")
endif()

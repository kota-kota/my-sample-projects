cmake_minimum_required(VERSION 3.10)
project(test_thread)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_thread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_timer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_queue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_threadpool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sample_async.cpp
)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    thread
)

if(MSVC)
  set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "thread")
endif()

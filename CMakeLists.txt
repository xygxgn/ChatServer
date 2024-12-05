cmake_minimum_required(VERSION 3.0.2)

project(chat)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/lib)

set(EXECUTABLE_OUTPUT_PATH 
  ${PROJECT_SOURCE_DIR}/bin
)

include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/thirdparty
)

add_subdirectory(src)
cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(wstp_example VERSION 0.1.0 LANGUAGES C CXX)

# Configure WSTP API paths if provided
option(WSTP_API_DIR "Path to the WSTP API" NULL)
if(NOT ${WSTP_API_DIR} STREQUAL "NULL")
  include_directories(${WSTP_API_DIR})
  link_directories(${WSTP_API_DIR})
endif()

add_executable(wstp_example Main.c)
target_link_libraries(wstp_example PRIVATE WSTPi4 c++)

if(APPLE)
  target_link_libraries(wstp_example PRIVATE "-framework Foundation")
endif()

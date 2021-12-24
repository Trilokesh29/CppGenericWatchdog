cmake_minimum_required(VERSION 3.10)

project(Watchdog)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++-10")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -ggdb -o0 -Wall -Werror")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_BUILD_TYPE Debug) 

include_directories(./inteface/ ./src)


add_executable(${PROJECT_NAME}
  src/main.cpp
  src/WatchdogMonitor.cpp
  src/WatchdogMonitor.h
  interface/IwatchdogMonitor.h
)

target_link_libraries(${PROJECT_NAME} pthread)
cmake_minimum_required(VERSION 3.10)

project(WatchdogTest)

find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++-10")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -ggdb -o0 -Wall -Werror -std=c++2a")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_BUILD_TYPE Debug) 


add_executable(${PROJECT_NAME}
  src/WatchdogMonitor.cpp
  src/WatchdogMonitor.h
  interface/IWatchdogMonitor.h
  test/main.cpp
  test/WatchdogTest.cpp
  test/WatchdogTest.h
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    interface
  PRIVATE
    src
    test
)

target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} pthread)
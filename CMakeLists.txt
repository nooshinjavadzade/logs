cmake_minimum_required(VERSION 3.10)

project(LogAnalyzer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(LogAnalyzer 
    src/main.cpp 
    src/Analyzer.cpp 
    src/Reporter.cpp 
    src/LogParser.cpp
)

target_include_directories(LogAnalyzer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
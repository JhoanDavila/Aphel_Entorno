cmake_minimum_required(VERSION 3.10)
project(AphelEngine VERSION 0.0.0.8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

file(GLOB_RECURSE SOURCES 
    "src/main.cpp"
    "src/ui/nodes/node/*.cpp"
    "src/ui/nodes/visual_node/*.cpp"
    "src/ui/nodes/2dspace/*.cpp"
    "src/ui/nodes/window/*.cpp"
    "src/ui/lector_aphelui/cleaner/*.cpp"
    "src/ui/lector_aphelui/tokenizer/*.cpp"
    "src/ui/lector_aphelui/parser/*.cpp"
    "src/ui/lector_aphelui/reader/*.cpp"
    "src/ui/lector_aphelui/error_manager/*.cpp"
)

add_executable(aphel ${SOURCES})
cmake_minimum_required(VERSION 3.12)
project(spaceDetector)

find_package(OpenCV REQUIRED)

option(DEBUG "Enable DEBUG macro" OFF)

add_executable(spaceDetector src/main.cpp src/forest.cpp src/segmentation.cpp src/spotLocator.cpp src/utils.cpp)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(spaceDetector PRIVATE ${OpenCV_LIBS})
target_compile_options(spaceDetector PRIVATE -O3)
target_compile_definitions(spaceDetector PRIVATE $<$<BOOL:${DEBUG}>:DEBUG>)
# cmake version
cmake_minimum_required(VERSION 3.0)

aux_source_directory(. DIR_SRCS)
add_executable(cv ${DIR_SRCS})

include_directories(${PROCESS_COMM_DIR})
target_link_libraries(cv process_communication)

#　OpenCV
find_package(OpenCV 3.4 REQUIRED)

if(OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
    target_link_libraries(cv ${OpenCV_LIBS})
endif()


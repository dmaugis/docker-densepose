cmake_minimum_required(VERSION 3.6)
project( texture2pose )
find_package(OpenCV REQUIRED )
find_package(Boost 1.55.0 COMPONENTS filesystem system REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(texture2pose_SOURCES	texture2pose.cpp)

include_directories(.)
include_directories(${OPENCV_INCLUDE_DIRS})	

add_executable( texture2pose ${texture2pose_SOURCES} )
target_link_libraries( texture2pose ${OpenCV_LIBS} ${Boost_LIBRARIES}   )
install(TARGETS texture2pose RUNTIME DESTINATION bin)	



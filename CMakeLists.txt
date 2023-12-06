cmake_minimum_required(VERSION 3.0.3)
project(vc_gps)

set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  serial
)

catkin_package()
roslaunch_add_file_check(launch)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(vc_gps_pub src/vc_gps_pub.cpp)
add_executable(vc_gps_pub2 src/vc_gps_pub2.cpp)

target_link_libraries(vc_gps_pub ${catkin_LIBRARIES})
target_link_libraries(vc_gps_pub2 ${catkin_LIBRARIES})

install(DIRECTORY  launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
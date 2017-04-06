cmake_minimum_required(VERSION 2.8)
project(octomap_ros)

find_package(catkin REQUIRED COMPONENTS sensor_msgs tf octomap_msgs)
find_package(octomap REQUIRED)
# 增加PCL库的依赖
FIND_PACKAGE( PCL REQUIRED COMPONENTS common io )

find_package(OpenCV 3.0 QUIET)
if(NOT OpenCV_FOUND)
   find_package(OpenCV 2.4.3 QUIET)
   if(NOT OpenCV_FOUND)
      message(FATAL_ERROR "OpenCV > 2.4.3 not found.")
   endif()
endif()

catkin_package(
  INCLUDE_DIRS include ${OCTOMAP_INCLUDE_DIRS}
  LIBRARIES ${PROJECT_NAME} ${OCTOMAP_LIBRARIES}
  DEPENDS sensor_msgs tf octomap_msgs)


# 添加头文件和库文件
ADD_DEFINITIONS( ${PCL_DEFINITIONS} )
INCLUDE_DIRECTORIES( ${PCL_INCLUDE_DIRS}  )
LINK_LIBRARIES( ${PCL_LIBRARY_DIRS} )

# octomap
#FIND_PACKAGE( octomap REQUIRED )
#INCLUDE_DIRECTORIES( ${OCTOMAP_INCLUDE_DIRS} )



include_directories(${catkin_INCLUDE_DIRS})
link_directories(${catkin_LIBRARY_DIRS})
link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES})

include_directories(${OCTOMAP_INCLUDE_DIRS})
link_directories(${OCTOMAP_LIBRARY_DIRS})
link_libraries(${PROJECT_NAME} ${OCTOMAP_LIBRARIES})

include_directories(include)
include_directories(include ${catkin_INCLUDE_DIRS})

add_library(${PROJECT_NAME} src/conversions.cpp)

add_executable(rcv_test
src/ros_listerner.cpp)
target_link_libraries(rcv_test ${PROJECT_NAME}
${PCL_LIBRARIES}
${OpenCV_LIBS} )

install(TARGETS ${PROJECT_NAME} 
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})

install(DIRECTORY include/octomap_ros/
  DESTINATION ${CATKIN_GLOBAL_INCLUDE_DESTINATION}/octomap_ros/
  FILES_MATCHING PATTERN "*.h"
  PATTERN ".svn" EXCLUDE)
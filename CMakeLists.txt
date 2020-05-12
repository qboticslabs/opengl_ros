cmake_minimum_required(VERSION 2.8.3)
project(opengl_ros)

#You have to execute the scripts/install_opencv_dev.sh before building this package
#Adding OpenCV customized build path
SET("OpenCV_DIR" "$ENV{HOME}/opencv_dev_install/opencv/build")

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  image_transport
  roscpp
  rospy
  sensor_msgs
  std_msgs
)


find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)



catkin_package()

#Removing ROS default opencv include path 
set (catkin_INCLUDE_DIRS /opt/ros/melodic/include;/opt/ros/melodic/share/xmlrpcpp/cmake/../../../include/xmlrpcpp;/usr/include)

#Removing ROS default opencv library path
set (catkin_LIBRARIES /opt/ros/melodic/lib/libcv_bridge.so;/opt/ros/melodic/lib/libimage_transport.so;/opt/ros/melodic/lib/libmessage_filters.so;/usr/lib/x86_64-linux-gnu/libtinyxml2.so;/opt/ros/melodic/lib/libclass_loader.so;/usr/lib/libPocoFoundation.so;/usr/lib/x86_64-linux-gnu/libdl.so;/opt/ros/melodic/lib/libroslib.so;/opt/ros/melodic/lib/librospack.so;/usr/lib/x86_64-linux-gnu/libpython2.7.so;/usr/lib/x86_64-linux-gnu/libboost_program_options.so;/usr/lib/x86_64-linux-gnu/libtinyxml.so;/opt/ros/melodic/lib/libroscpp.so;/usr/lib/x86_64-linux-gnu/libboost_filesystem.so;/usr/lib/x86_64-linux-gnu/libboost_signals.so;/opt/ros/melodic/lib/librosconsole.so;/opt/ros/melodic/lib/librosconsole_log4cxx.so;/opt/ros/melodic/lib/librosconsole_backend_interface.so;/usr/lib/x86_64-linux-gnu/liblog4cxx.so;/usr/lib/x86_64-linux-gnu/libboost_regex.so;/opt/ros/melodic/lib/libxmlrpcpp.so;/opt/ros/melodic/lib/libroscpp_serialization.so;/opt/ros/melodic/lib/librostime.so;/opt/ros/melodic/lib/libcpp_common.so;/usr/lib/x86_64-linux-gnu/libboost_system.so;/usr/lib/x86_64-linux-gnu/libboost_thread.so;/usr/lib/x86_64-linux-gnu/libboost_chrono.so;/usr/lib/x86_64-linux-gnu/libboost_date_time.so;/usr/lib/x86_64-linux-gnu/libboost_atomic.so;/usr/lib/x86_64-linux-gnu/libpthread.so;/usr/lib/x86_64-linux-gnu/libconsole_bridge.so
)

include_directories(
  ${catkin_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} 
)


add_executable(opengl_ros src/cv_gl_ros.cpp)
target_link_libraries(opengl_ros PRIVATE ${catkin_LIBRARIES} ${OpenCV_LIBS} ${OPENGL_LIBRARIES})




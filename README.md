octomap_ros
===========
1、Based on the things below:
ROS package to provide conversion functions between ROS and OctoMap's native types.

This repository continues from the groovy branch imported from SVN: 
http://alufr-ros-pkg.googlecode.com/svn/branches/octomap_stacks-groovy-devel/octomap_ros/ 
See https://code.google.com/p/alufr-ros-pkg/ for the previous versions.

2、Compile:
mkdir build/
cd build/
cmake ..
make

3、Usage:(command -->explanation)
rosrun octomap_ros rcv_test -->receiving data--"/ORB_SLAM/pointcloud2" and transforming it into the octomap format
octovis sample.ot -->view it in the octovis.To get more information about octovis, you can refer to http://wiki.ros.org/octovis.

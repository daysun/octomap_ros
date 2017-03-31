octomap_ros
===========
1、Based on the things below:<br/>
ROS package to provide conversion functions between ROS and OctoMap's native types.<br/><br/>

This repository continues from the groovy branch imported from SVN: <br/>
http://alufr-ros-pkg.googlecode.com/svn/branches/octomap_stacks-groovy-devel/octomap_ros/ <br/>
See https://code.google.com/p/alufr-ros-pkg/ for the previous versions.<br/>

2、Compile:<br/>
mkdir build/ <br/>
cd build/ <br/>
cmake ..<br/>
make<br/><br/>

3、Usage:(command -->explanation)<br/>
rosrun octomap_ros rcv_test -->receiving data--"/ORB_SLAM/pointcloud2" and transforming it into the octomap format<br/>
octovis sample.ot -->view it in the octovis.To get more information about octovis, you can refer to http://wiki.ros.org/octovis.<br/>

octomap_ros
===========
<b>Make sure you have installed all these below.</b>
1、OpenCV<br/>
You can get information about this in http://docs.opencv.org/master/d9/df8/tutorial_root.html<br/>
2、ROS<br/>
You can get information about this in http://wiki.ros.org/indigo/Installation/Ubuntu<br/>
3、octomap && octovis<br/>
You can get information about this in https://github.com/OctoMap/octomap<br/>
To get more information about octovis, you can refer to http://wiki.ros.org/octovis<br/>
4、PCL<br/>
You can get information about this in http://pointclouds.org/downloads/<br/>

<b>Compile</b><br/>
You should put this project under your ROS work space, such as your_ros_path/src/<br/>
cd your_ros_path<br/>
catkin_make<br/>

<b>Usage</b>
1、 rosrun octomap_ros rcv_test<br/>
Receive msg(Type: Id_PointCloud2) from ROS, which named “/ORB_SLAM/pointcloud2” .<br/>
It'll transform the msg into the octomap format.<br/>
Try "Ctrl+C" to stop it, which would generate origin.ot.You can view it in octovis.<br/>
octovis  origin.ot<br/>
2、 rosrun octomap_ros search_test<br/>
Test test_search.cpp, can use alone.<br/>

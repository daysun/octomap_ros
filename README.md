octomap_ros
===========
<b>Make sure you have installed all these below.</b><br/>
1、OpenCV<br/>
&ensp;&ensp;You can get information about this in http://docs.opencv.org/master/d9/df8/tutorial_root.html<br/>
2、ROS<br/>
  &ensp;&ensp; You can get information about this in http://wiki.ros.org/indigo/Installation/Ubuntu<br/>
3、octomap && octovis<br/>
 &ensp;&ensp;  You can get information about this in https://github.com/OctoMap/octomap<br/>
 &ensp;&ensp;  To get more information about octovis, you can refer to http://wiki.ros.org/octovis<br/>
4、PCL<br/>
  &ensp;&ensp; You can get information about this in http://pointclouds.org/downloads/<br/>

<b>Compile</b><br/>
You should put this project under your ROS work space, such as <i>your_ros_path/src/</i><br/>
<i>cd your_ros_path</i><br/>
<i>catkin_make</i><br/>

<b>Usage</b><br/>
1、 <i>rosrun octomap_ros rcv_test</i><br/>
&ensp;&ensp;Receive msg(Type: Id_PointCloud2) from ROS, which named “/ORB_SLAM/pointcloud2” .<br/>
&ensp;&ensp;It'll transform the msg into the octomap format.<br/>
&ensp;&ensp;Try "Ctrl+C" to stop it, which would generate origin.ot.You can view it in octovis.<br/>
&ensp;&ensp;<i>octovis  origin.ot</i><br/>
2、 <i>rosrun octomap_ros search_test</i><br/>
&ensp;&ensp;Test test_search.cpp, can use alone.<br/>

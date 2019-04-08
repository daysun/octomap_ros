octomap_ros
===========
<b>Related Publications:</b><br/>
    @inproceedings{Yang2017Real,<br/>
     &ensp; title={Real-time Globally Consistent 3D Grid Mapping},<br/>
     &ensp; author={Yang, Sining and Yang, Shaowu and Yi, Xiaodong},<br/>
     &ensp; booktitle={IEEE International Conference on Robotics and Biomimetics},<br/>
     &ensp; year={2017},<br/>
    }<br/>
    
<b>Dependencies</b><br/>
1、OpenCV(http://docs.opencv.org/master/d9/df8/tutorial_root.html)<br/>
<b>2、The modified octomap(https://github.com/daysun/octomap/) && octovis(http://wiki.ros.org/octovis)</b><br/>
3、PCL(http://pointclouds.org/downloads/)<br/>
4、genmsg(https://github.com/ros/genmsg)<br/>
5、octomap_msgs(https://github.com/OctoMap/octomap_msgs/tree/indigo-devel)<br/>
6、orb-slam2(git@github.com:daysun/ORB-SLAM2.git)<br/>

<b>Compile</b><br/>
You should put this project under your ROS work space, such as <i>your_ros_path/src/</i><br/>
<i>cd your_ros_path</i><br/>
<i>catkin_make</i><br/>

<b>Usage</b><br/>
1、 <i>rosrun octomap_ros rcv_test</i><br/>
&ensp;&ensp;Before executing this, make sure ORB-SLAM2(https://github.com/daysun/ORB-SLAM2) are running.<br/>
&ensp;&ensp;The experimental rosbag data can be attained in https://pan.baidu.com/s/1boC6X7h, and the corresponding parameters can be attained in /data/TUM1.yaml.<br/>

&ensp;&ensp;This procedure receives msg(Type: Id_PointCloud2) from ROS, which named “/ORB_SLAM/pointcloud2” . And it will transform the msg into the octomap format.<br/>
&ensp;&ensp;Try "Ctrl+C" to stop it any time you want, which would generate origin.ot. You can view it in octovis.<br/>
&ensp;&ensp;<i>octovis  origin.ot</i><br/><br/>
2、 <i>rosrun octomap_ros runtime_test</i><br/>
&ensp;&ensp;For experiments.<br/>
&ensp;&ensp;Show that our system can perform on-line. The usage is the same with the former one.<br/>
&ensp;&ensp;And the results built by original grid mapping, original grid mapping and local grid mapping, and complete update strategy using our experimental data can be attained in /data/*.ot.<br/>
&ensp;&ensp;The experiments are also shown in a vedio online(http://v.youku.com/v_show/id_XMjg5MTA1OTg4OA).<br/>

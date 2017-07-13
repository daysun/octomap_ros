#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/point_cloud2_iterator.h>
#include <assert.h>
#include <cstddef>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "octomap_ros/Id_PointCloud2.h"
#include "octomap_ros/loopId_PointCloud2.h"
//pcl
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl/conversions.h>
#include <pcl_ros/transforms.h>
#include <pcl/io/io.h>
 #include <pcl/visualization/cloud_viewer.h>
#include <pcl/visualization/pcl_visualizer.h>
//octomap
#include <octomap/octomap.h>
#include "octomap_ros/ColorOcTree.h"
#include<octomap/OcTreeNode.h>
#include <string>
#include <fstream>
using namespace std;

///receive data(orb-slam published it) from ros
/// saved it as sample.ot
octomap::ColorOcTree tree( 0.1 );
int loopNum = -1;

//write into file
ofstream outfile("/home/daysun/prune.txt", ofstream::app);
//ofstream outidset("/home/daysun/idSetTime.txt", ofstream::app);
 //   string temp;

//ros::Duration allTime;

int countKF = 0;
///initial insert
void chatterCallback(const octomap_ros::Id_PointCloud2::ConstPtr & my_msg)
{
//      ros::Time tB = ros::Time::now();
//    cout<<my_msg->kf_id<<endl;
    pcl::PCLPointCloud2 pcl_pc2;
    pcl_conversions::toPCL(my_msg->msg,pcl_pc2);
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr temp_cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
    pcl::fromPCLPointCloud2(pcl_pc2,*temp_cloud);
    for (int i=0;i<temp_cloud->points.size();i++)
    {
        tree.updateNode(octomap::point3d(temp_cloud->points[i].x,temp_cloud->points[i].y,temp_cloud->points[i].z),true);
     }

    for  (int i=0;i<temp_cloud->points.size();i++)
    {
        tree.IntegrateNodeColor( temp_cloud->points[i].x,temp_cloud->points[i].y,temp_cloud->points[i].z,
                                 temp_cloud->points[i].r,temp_cloud->points[i].g,temp_cloud->points[i].b);
        tree.integrateNodeId(temp_cloud->points[i].x,temp_cloud->points[i].y,temp_cloud->points[i].z,
                             my_msg->kf_id);
    }

//    countKF +=1;
//    if(countKF == 20){
//        cout<<"updateInnerOccupancy + pruneTree every 20KF\n";
    tree.updateInnerOccupancy();
      ros::Time t1 = ros::Time::now();
    tree.pruneTree(tree.getRoot(), 0);
     ros::Time t2 = ros::Time::now();
    ros::Duration bTcreate = t2-t1;
    outfile<<bTcreate.toSec()*1000<<"\t";  //micro sec
    cout<<"update kfid-set time:"<<bTcreate.toSec()*1000<<endl;
//    ros::Time t2 = ros::Time::now();
//     ros::Duration bTcreate = t2-t1;
//     ros::Duration bTcreate2 = t1-t0;
//     outidset<<bTcreate2.toSec()*1000<<"\t";  //id-set
//     outfile<<bTcreate.toSec()*1000<<"\t";  //micro sec --prune
//     cout<<"prune time:"<<bTcreate.toSec()<<endl;
//    ros::Time t2 = ros::Time::now();
//     ros::Duration bTcreate = t2-t1;
//     cout<<"initial update time:"<<bTcreate.toSec()<<endl;
//    countKF = 0;
//    }
//      std::cout << "initial update:" <<tDel1.toSec()<< std::endl;
//    ros::Duration bTcreate = ros::Time::now() - tB;
//    allTime += bTcreate;
//      std::cout << "time cost per key frame: " << bTcreate.toSec() << std::endl; //0.04
}

///after ORB-SLAM local update
void chatterCallback_local(const octomap_ros::Id_PointCloud2::ConstPtr & my_msg)
{
   //  ros::Time t1 = ros::Time::now();
   tree.deleteById(my_msg->kf_id);
     //for testing
 //    tree.deleteIdWIthoutSet2(my_msg->kf_id);
     //   ros::Time t2 = ros::Time::now();
//         ros::Duration bTcreate = t2-t1;
//         outfile<<bTcreate.toSec()*1000<<"\t";  //micro sec
//         cout<<"id, prune time:"<<bTcreate.toSec()*1000<<endl;
         // OcTreeNode* node = tree.search(key);
         //use the original methods


    //add new pointCloud-0.03
    pcl::PCLPointCloud2 pcl_pc2;
    pcl_conversions::toPCL(my_msg->msg,pcl_pc2);
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr temp_cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
    pcl::fromPCLPointCloud2(pcl_pc2,*temp_cloud);
    for (int i=0;i<temp_cloud->points.size();i++)
    {
        tree.updateNode(octomap::point3d(temp_cloud->points[i].x,temp_cloud->points[i].y,temp_cloud->points[i].z),true);
     }

    for  (int i=0;i<temp_cloud->points.size();i++)
    {
        tree.IntegrateNodeColor( temp_cloud->points[i].x,temp_cloud->points[i].y,temp_cloud->points[i].z,
                                 temp_cloud->points[i].r,temp_cloud->points[i].g,temp_cloud->points[i].b);
        tree.integrateNodeId(temp_cloud->points[i].x,temp_cloud->points[i].y,temp_cloud->points[i].z,
                             my_msg->kf_id);
    }
    //    tree.updateInnerOccupancy();
    //    tree.pruneTree(tree.getRoot(), 0);
}

void chatterCallback_global(const octomap_ros::loopId_PointCloud2::ConstPtr & my_msg){
    //delete the old tree
    if(loopNum == -1){
        //the first time
        tree.deleteTree();
        loopNum = my_msg->loop_id;
    }else{
        //not the first time
        if(loopNum != my_msg->loop_id){
            tree.deleteTree();
            loopNum = my_msg->loop_id;
        }
    }

    pcl::PCLPointCloud2 pcl_pc2;
    pcl_conversions::toPCL(my_msg->msg,pcl_pc2);
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr temp_cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
    pcl::fromPCLPointCloud2(pcl_pc2,*temp_cloud);
    for (int i=0;i<temp_cloud->points.size();i++)
    {
        tree.updateNode(octomap::point3d(temp_cloud->points[i].x,temp_cloud->points[i].y,temp_cloud->points[i].z),true);
     }

    for  (int i=0;i<temp_cloud->points.size();i++)
    {
        tree.IntegrateNodeColor( temp_cloud->points[i].x,temp_cloud->points[i].y,temp_cloud->points[i].z,
                                 temp_cloud->points[i].r,temp_cloud->points[i].g,temp_cloud->points[i].b);
        tree.integrateNodeId(temp_cloud->points[i].x,temp_cloud->points[i].y,temp_cloud->points[i].z,
                             my_msg->kf_id);
    }
}

void coutInnerOccupancy(octomap::ColorOcTreeNode* node, unsigned int depth,int tree_depth) {
  if (node->hasChildren()){
    if (depth < tree_depth){
      for (unsigned int i=0; i<8; i++) {
        if (node->childExists(i)) {
          coutInnerOccupancy(node->getChild(i), depth+1,tree_depth);
        }
      }
    }
    cout<<node->getLogOdds()<<"\n";
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "ORB_SLAM2_runtime_listener2");
  ros::start();

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("/ORB_SLAM/pointcloud2", 1000, chatterCallback);
  ros::Subscriber sub_change = n.subscribe("ORB_SLAM/pointcloudlocalup2", 1000, chatterCallback_local);
  ros::Subscriber sub_global = n.subscribe("ORB_SLAM/pointcloudup2", 1000, chatterCallback_global);

  ros::spin();
  ros::shutdown();
//  std::cout << "time cost all: " << allTime.toSec() << std::endl; //hpcl_office_asuse.bag-34.388

  //test updateOccupancyChildren--test the children's log-odds
  //int tree_depth = tree.getTreeDepth();
  //coutInnerOccupancy(tree.getRoot(),0,tree_depth);

  tree.updateInnerOccupancy();
//  tree.pruneTree(tree.getRoot(), 0);
  outfile.close();
  tree.write( "origin.ot" );
  cout<<"done."<<endl;

  return 0;
}

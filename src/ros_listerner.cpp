#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/point_cloud2_iterator.h>
#include <octomap_ros/conversions.h>
#include <assert.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
//pcl
#include <pcl/io/pcd_io.h>
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
#include <octomap/ColorOcTree.h>
using namespace std;


///receive data(orb-slam published it) from ros
/// saved it as sample.ot
octomap::ColorOcTree tree( 0.05 );
octomap::ColorOcTree tree_change( 0.05 );

void chatterCallback(const sensor_msgs::PointCloud2::ConstPtr & msg)
{
       pcl::PCLPointCloud2 pcl_pc2;
        pcl_conversions::toPCL(*msg,pcl_pc2);
        pcl::PointCloud<pcl::PointXYZRGB>::Ptr temp_cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
        pcl::fromPCLPointCloud2(pcl_pc2,*temp_cloud);
         octomap::Pointcloud cloud_octo;

        for (int i=0;i<temp_cloud->points.size();i++)
        {
            cloud_octo.push_back(temp_cloud->points[i].x,temp_cloud->points[i].y,temp_cloud->points[i].z);
        }
        tree.insertPointCloud( cloud_octo,octomap::point3d(1,1,1));
        //tree.insertPointCloud( cloud_octo,true);

        for  (int i=0;i<temp_cloud->points.size();i++)
        {
            tree.integrateNodeColor( temp_cloud->points[i].x,temp_cloud->points[i].y,temp_cloud->points[i].z,
                                     temp_cloud->points[i].r,temp_cloud->points[i].g,temp_cloud->points[i].b);
        }
        tree.updateInnerOccupancy();
}

void chatterCallback_change(const sensor_msgs::PointCloud2::ConstPtr & msg)
{
       pcl::PCLPointCloud2 pcl_pc2;
        pcl_conversions::toPCL(*msg,pcl_pc2);
        pcl::PointCloud<pcl::PointXYZRGB>::Ptr temp_cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
        pcl::fromPCLPointCloud2(pcl_pc2,*temp_cloud);
         octomap::Pointcloud cloud_octo;

        for (int i=0;i<temp_cloud->points.size();i++)
        {
            cloud_octo.push_back(temp_cloud->points[i].x,temp_cloud->points[i].y,temp_cloud->points[i].z);
        }
        tree_change.insertPointCloud( cloud_octo,octomap::point3d(1,1,1));

        for  (int i=0;i<temp_cloud->points.size();i++)
        {
            tree_change.integrateNodeColor( temp_cloud->points[i].x,temp_cloud->points[i].y,temp_cloud->points[i].z,
                                     temp_cloud->points[i].r,temp_cloud->points[i].g,temp_cloud->points[i].b);
        }
        tree_change.updateInnerOccupancy();
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "ORB_SLAM2_listener");
  ros::start();

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("/ORB_SLAM/pointcloud2", 1000, chatterCallback);
  //ros::Subscriber sub_change = n.subscribe("/ORB_SLAM/pointcloudfull2", 1000, chatterCallback_change);

  ros::spin();
  ros::shutdown();
  tree.write( "origin.ot" );
  //tree_change.write( "change.ot" );
  cout<<"done."<<endl;

  return 0;
}

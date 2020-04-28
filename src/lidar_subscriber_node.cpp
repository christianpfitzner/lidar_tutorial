/* Copyright (C) 2020 Prof. Dr. Christian Pfitzner - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the BSD license, which unfortunately won't be
 * written for another century.
 */





/*
 * topics: 
 * - working with lidar topics
 * - publishing markers
 */



// ros includes
#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <visualization_msgs/Marker.h>

// std cpp includes
#include <algorithm>
#include <vector>



ros::Publisher vis_pub; 




void lidarCallback(const sensor_msgs::LaserScan::ConstPtr scan)
{
  // calculate here the Cartesian point which is closest to the scanners origin

  ROS_INFO_STREAM("***********"); 
  ROS_INFO_STREAM("Frame id: " << scan->header.frame_id); 





  const auto closest_idx = std::min_element(scan->ranges.begin(), scan->ranges.end());
  const auto idx         = std::distance(scan->ranges.begin(), closest_idx);
  const auto angle       = scan->angle_min + scan->angle_increment*idx; 


  ROS_INFO_STREAM("closest idx: "      << idx);
  ROS_INFO_STREAM("closest distance: "        << scan->ranges[idx]);
  ROS_INFO_STREAM("angle for closest point: " << angle); 

  const auto x = scan->ranges[idx] * cos(angle); 
  const auto y = scan->ranges[idx] * sin(angle); 


  ROS_INFO_STREAM("closest point with coordinates (x, y): " << x << ", " << y); 



  // now we gernate a marker for the closest point
  visualization_msgs:: Marker marker;
  marker.header.frame_id    = "laser";
  marker.header.stamp       = ros::Time();
  marker.ns                 = "closest_obstacle";
  marker.id                 = 0;
  marker.type               = visualization_msgs::Marker::SPHERE;
  marker.action             = visualization_msgs::Marker::ADD;
  marker.pose.position.x    = x;
  marker.pose.position.y    = y;
  marker.pose.position.z    = 0;
  marker.pose.orientation.x = 0.0;
  marker.pose.orientation.y = 0.0;
  marker.pose.orientation.z = 0.0;
  marker.pose.orientation.w = 1.0;
  marker.scale.x            = 0.1;
  marker.scale.y            = 0.1;
  marker.scale.z            = 0.1;
  marker.color.a            = 1.0;                                 // Don't forget to set the alpha!
  marker.color.r            = 1.0;
  marker.color.g            = 0.0;
  marker.color.b            = 0.0;



  // hier müssen Sie noch das Versenden des Markers über den bestehenden Publisher einfügen. 


}



int main(int argc, char **argv)
{
  ros::init(argc, argv, "lidar_marker_node");
  ros::NodeHandle n;



  ros::Subscriber scan_sub = n.subscribe("scan", 1, lidarCallback);
  vis_pub                  = n.advertise<visualization_msgs::Marker>( "visualization_marker", 0 );


  ros::spin(); 
}
/*
 * @Description: 
 * @Author: sunm
 * @Github: https://github.com/sunmiaozju
 * @LastEditors: sunm
 * @Date: 2019-02-21 21:41:21
 * @LastEditTime: 2019-02-21 21:58:42
 */
#ifndef NODE_JOINT_PIXEL_POINTCLOUD_H
#define NODE_JOINT_PIXEL_POINTCLOUD_H

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include <ros/ros.h>
#include <tf/tf.h>
#include <tf/transform_listener.h>

#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/PointCloud.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/CameraInfo.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include <yunle_sensor_msgs/DetectObject.h>
#include <yunle_sensor_msgs/DetectObjs.h>
#include <smartcar_msgs/DetectedObjectArray.h>
#include <smartcar_msgs/DetectedObject.h>

#include <opencv2/opencv.hpp>
#include <pcl/PCLPointCloud2.h>
#include <pcl_ros/point_cloud.h>

namespace NODE_JOINT_PIXEL_POINTCLOUD
{

class Object
{
public:
  float score;
  int category;
  int xmin, ymin, xmax, ymax;
  int xmin_3d_pic, ymin_3d_pic, xmax_3d_pic, ymax_3d_pic;
  float obj_deg;
  float obj_dist;
  float xmin_3d_bbox, ymin_3d_bbox, zmin_3d_bbox, xmax_3d_bbox, ymax_3d_bbox, zmax_3d_bbox;
};

class PixelCloudFusion
{
  ros::NodeHandle nh;
  ros::NodeHandle nh_private;
  ros::Publisher pub_fusion_cloud;
  ros::Subscriber sub_intrinsics;
  ros::Subscriber sub_image;
  ros::Subscriber sub_cloud;
  ros::Subscriber sub_detection;
  ros::Publisher transformed_pointcloud;
  ros::Publisher objs_pub_rviz;
  ros::Publisher objs_pub;

  std::vector<Object> objs;

  cv::Size image_size;
  cv::Mat camera_instrinsics; //相机内参
  cv::Mat current_frame;
  cv::Mat distortion_coefficients; //畸变系数

  tf::StampedTransform camera_lidar_tf;
  tf::TransformListener transform_listener;

  bool camera_lidar_tf_ok_;
  bool camera_info_ok_;

  std::string image_frame_id;

  float fx, fy, cx, cy;

  pcl::PointCloud<pcl::PointXYZRGB> colored_cloud;

  void ImageCallback(const sensor_msgs::Image::ConstPtr &image_msg);

  void CloudCallback(const sensor_msgs::PointCloud2::ConstPtr &cloud_msg);

  void IntrinsicsCallback(const sensor_msgs::CameraInfo &intrinsics_msg);

  void DetectionCallback(const yunle_sensor_msgs::DetectObjs &objs_msg);

  void initROS();

  void publishObjs();

  tf::StampedTransform FindTransform(const std::string &target_frame, const std::string source_frame);

  pcl::PointXYZ TransformPoint(const pcl::PointXYZ &in_point, const tf::StampedTransform &in_transform);

  void category_deal(visualization_msgs::Marker &objmarker, Object &obj);

public:
  PixelCloudFusion();
};
} // namespace NODE_JOINT_PIXEL_POINTCLOUD

#endif //NODE_JOINT_PIXEL_POINTCLOUD

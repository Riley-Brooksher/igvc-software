#ifndef LINEDETECTOR_H
#define LINEDETECTOR_H
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <image_transport/image_transport.h>
#include <image_geometry/pinhole_camera_model.h>
#include <cv_bridge/cv_bridge.h>
#include <tf/transform_listener.h>

#define KERNAL_COUNT 8

class LineDetector
{
public:
    LineDetector(ros::NodeHandle &handle, const std::string& topic);
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud;

private:
    cv::Mat src_img, working, fin_img;

    void info_img_callback(const sensor_msgs::ImageConstPtr& msg, const sensor_msgs::CameraInfoConstPtr& cam_info);
    void img_callback(const sensor_msgs::ImageConstPtr& msg);


    // ROS COMMUNICATION
    image_transport::ImageTransport _it;
    std::string topic;
    image_transport::Publisher _filt_img;
    image_transport::Subscriber _src_img;
    image_transport::CameraSubscriber _src_img_info;
    ros::Publisher _line_cloud;
    tf::TransformListener tf_listener;
    image_geometry::PinholeCameraModel cam;
    bool hasInfo = false;
};
#endif // LINEDETECTOR_H

#include "linedetector.h"
#include <pcl_ros/point_cloud.h>
#include <igvc/CVUtils.hpp>

cv_bridge::CvImagePtr cv_ptr;
typedef pcl::PointCloud<pcl::PointXYZ> PCLCloud;

void LineDetector::info_img_callback(const sensor_msgs::ImageConstPtr& msg, const sensor_msgs::CameraInfoConstPtr& cam_info) {
    cam.fromCameraInfo(cam_info);
    img_callback(msg);
}

void LineDetector::img_callback(const sensor_msgs::ImageConstPtr& msg) {
    cv_ptr = cv_bridge::toCvCopy(msg, "");
    src_img = cv_ptr->image;

    cv::resize(src_img, src_img, cv::Size(524, 524), 0, 0, CV_INTER_AREA);
    fin_img = cv::Mat::zeros(src_img.size(), src_img.type());

    cv::GaussianBlur(src_img, working, cv::Size(0,0), 2.0);

    cv::Canny(working, working, 45, 135, 3);

    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(working, lines, 1.0, CV_PI/180, 80, 35, 15);
    for (size_t i = 0; i < lines.size(); ++i) {
        line(fin_img, cv::Point(lines[i][0], lines[i][1]), cv::Point(lines[i][2], lines[i][3]), cv::Scalar(255, 255, 255), 3, 8);
    }

    if (hasInfo) {
        cloud = toPointCloud(tf_listener, MatToContours(fin_img), cam);
    } else {
        cloud = toPointCloud(tf_listener, MatToContours(fin_img), fin_img.size().height, fin_img.size().width);
    }
    _line_cloud.publish(cloud);

    cv_ptr->image = fin_img;
    _filt_img.publish(cv_ptr->toImageMsg());
}

LineDetector::LineDetector(ros::NodeHandle &handle, const std::string& topic)
      : _it(handle)
      , topic(topic)
      , tf_listener(handle)
{
     if (!hasInfo) {
        _src_img = _it.subscribe("stereo/left/image_raw", 1, &LineDetector::img_callback, this);
     } else {
        _src_img_info = _it.subscribeCamera(topic + "/image_raw", 1, &LineDetector::info_img_callback, this);
     }
    _filt_img = _it.advertise(topic + "/filt_img", 1);
    _line_cloud = handle.advertise<PCLCloud>(topic + "/line_cloud", 100);
}



#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;

image_transport::Publisher pub;
void imageCallBack(const sensor_msgs::ImageConstPtr& msg){
    cv::Mat frame;
    cv::Mat edge ;
    cv::Mat original;
    int key;
    try
    {
        /* code */
        frame = cv_bridge::toCvShare(msg,"mono8")->image;
        
        cv::Canny(frame,edge,75,150);
        
        if(!frame.empty()){
            cv::imshow("video_node2",edge);
            if(cv::waitKey(100) == 27){
                cv::destroyWindow("video_node2");
            }
        }
        else{
            cv::destroyWindow("video_node2");
            return;
            }
        
        key = cv::waitKey(100)&0xff;
    }
    catch(cv_bridge::Exception& e)
    {
        ROS_ERROR("could not convert '%s' to 'bgr8'",msg->encoding.c_str());
    }
    if(key=='q'){
            cv::destroyAllWindows();
             }
    sensor_msgs::ImagePtr msg2;
    msg2 = cv_bridge::CvImage(std_msgs::Header(),"mono8",edge).toImageMsg();
    pub.publish(msg2);
}
int main(int argc ,char **argv){
    ros::init(argc,argv,"img_node2");

    ros::NodeHandle n2;
    cv::namedWindow("video_node2",cv::WINDOW_AUTOSIZE);
    //cv::startWindowThread();
    image_transport::ImageTransport it(n2);
    image_transport::Subscriber sub = it.subscribe("video1",1,imageCallBack);
    pub=it.advertise("video2",1);
    ros::spin();
    cv::waitKey(100);
    cv::destroyWindow("video_node2");
    return 0;


}

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv)
{
    ros::init(argc, argv,"image_publisher");

    ros::NodeHandle n1;
    image_transport::ImageTransport it(n1);

    image_transport::Publisher pub = it.advertise("video1" ,1);
    image_transport::Publisher pub2 = it.advertise("original_video",1);

    
    cv::VideoCapture cap("/media/malayajrath/New Volume/code testing/python/main video.mp4");
    if(!cap.isOpened()){
        cout<<"Unable to open video file"<<endl;
        return 1;
    }
    cv::Mat frame;
    sensor_msgs::ImagePtr msg;
    sensor_msgs::ImagePtr msg2;
    ros::Rate loop_rate(50);
    while(n1.ok()){
        cap.read(frame);
        if(!frame.empty()){
            cv::Mat blur;
            cv::GaussianBlur(frame,blur,cv::Size(7,7),0);
            cv::Mat HSVmask;
            cv::cvtColor(blur,HSVmask,cv::COLOR_BGR2HSV);
            cv::Mat mask;
            cv::inRange(HSVmask,cv::Scalar(20,80,130),cv::Scalar(39,255,255),mask);
            msg = cv_bridge::CvImage(std_msgs::Header(),"mono8",mask).toImageMsg();
            msg2 = cv_bridge::CvImage(std_msgs::Header(),"bgr8",frame).toImageMsg();
            pub.publish(msg);
            pub2.publish(msg2);
            //cv::imshow("origianl",frame);
            //cv::imshow("blur",mask);
            if(cv::waitKey(100) == 27){
                break;
            }
            
        }
        else{
            cv::destroyAllWindows();
        }
        ros::spinOnce();
        loop_rate.sleep();
        
    }
    cap.release();
    cv::destroyAllWindows();
    return 0;
}
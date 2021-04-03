#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <sstream>
#include <bits/stdc++.h>

using namespace std;

int main(int argc , char **argv)
{
    ros::init(argc,argv,"ball_node1");

    ros::NodeHandle nh;
    
    image_transport::ImageTransport it(nh);

    image_transport::Publisher pub1 = it.advertise("video1",1);
    image_transport::Publisher pub2 = it.advertise("original",1);

    cv::VideoCapture cap("/media/malayajrath/New Volume/code testing/python/red ball green background.mp4"); // reading the video 

    if(!cap.isOpened()){   // check if it is opening
        ROS_ERROR("unable open video file"); 
        return 1;
    }

    cv::Mat frame ;

    sensor_msgs::ImagePtr msg1;
    sensor_msgs::ImagePtr msg2;

    ros::Rate loop_rate(50);

    cap.read(frame);

    cv::namedWindow("tracker",cv::WINDOW_AUTOSIZE);

    int hmin=27,hmax=179,smin=62,smax=241,vmin=42,vmax=255;
    
     cv::createTrackbar("h_min","tracker",&hmin,179);
     cv::createTrackbar("h_max","tracker",&hmax,179);
     cv::createTrackbar("s_min","tracker",&smin,255);
     cv::createTrackbar("s_max","tracker",&smax,255);
     cv::createTrackbar("v_min","tracker",&vmin,255);
     cv::createTrackbar("v_max","tracker",&vmax,255);
    while (nh.ok() and !frame.empty())
    {
        cap.read(frame);

        if(!frame.empty())
        {
            cv::Mat hsv;
            cv::cvtColor(frame,hsv,cv::COLOR_BGR2HSV);

            cv::Mat blur;
            cv::GaussianBlur(hsv, blur,cv::Size(9,9),1,1);

            cv::Scalar lower(hmin,smin,vmin);
            cv::Scalar upper(hmax,smax,vmax);

            cv::Mat hsvmask;
            cv::inRange(blur,lower,upper,hsvmask);
            
            cv::Mat mask;
            cv::bitwise_and(frame,frame,mask,hsvmask);

            msg1 = cv_bridge::CvImage(std_msgs::Header(),"mono8",hsvmask).toImageMsg();
            pub1.publish(msg1);
            
            msg2 = cv_bridge::CvImage(std_msgs::Header(),"bgr8",frame).toImageMsg();
            pub2.publish(msg2);

            
            cv::imshow("mask_frame",mask);
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
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

// cv::Mat original_frame ;
int key;
vector<cv::Vec4i> lines;
vector<int> thethas;
int th;
int x;
int y;
int M ;
int m;

// cv::VideoCapture cap ("/media/malayajrath/New Volume/code testing/python/main video.mp4");

void imgcallback(const sensor_msgs::ImageConstPtr& msg){
    cv::Mat frame ;
    //cap.read(original_frame);
    
    try
    {
        /* code */
        frame = cv_bridge::toCvShare(msg,"mono8")->image;
        cv::HoughLinesP(frame,lines,1,CV_PI/180,40,10,30);
        if(!lines.empty()){
            x=0;
            y=0;
            thethas.clear();
            for(int i =0 ; i<lines.size(); i++){
                auto l = lines[i];
                x+=l[0]+l[2];
                y+=l[1]+l[3];
                if(l[0]-l[2]==0){
                    th = 90;
                }
                else{
                    cout<<"X1="<<l[0]<<endl;
                    cout<<"Y1="<<l[1]<<endl;
                    cout<<"X2="<<l[2]<<endl;
                    cout<<"Y2="<<l[3]<<endl;
                    th=atan((l[3]-l[1])/(l[2]-l[0]))*(180/3.14);
                    
                    cout<<"th="<<th<<endl;
                }
                thethas.push_back(th);
            }
        M =  *max_element(thethas.begin(), thethas.end());
        m =  *min_element(thethas.begin(), thethas.end());
        cout<<"M="<<M<<"  "<<"m="<<m<<endl;
        x = (int)(0.5*x)/(thethas.size());
        y = (int)(0.5*y)/(thethas.size());
        cout<<"x="<<x<<"  "<<"y="<<y<<endl;
        
            //cv::line( frame, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 3, CV_AA);
        }
        
        if(!frame.empty()){
            cv::imshow("video_node3",frame);
            if(cv::waitKey(100)==27){
                cv::destroyAllWindows();
            }
        }
        else{
            cv::destroyAllWindows();
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

}


void imgcallback2(const sensor_msgs::ImageConstPtr& msg1){
        cv::Mat frame;
        frame = cv_bridge::toCvShare(msg1,"bgr8")->image;
        // int x=0;
        // int y=0;
         for(int i =0 ; i<lines.size(); i++){
             auto L = lines[i];
        //     x+=l[0]+l[2];
        //     y+=l[1]+l[3];
        //     if(l[0]-l[2]==0){
        //         th = 3.14/2;
        //     }
        //     else{
        //        th=atan((l[3]-l[1])/(l[2]-l[0]))*(180/3.14);
        //     }
        //     thethas.push_back(th);
             cv::line( frame, cv::Point(L[0], L[1]), cv::Point(L[2], L[3]), cv::Scalar(0,0,255), 3, CV_AA);
        }
        //     int M =  *max_element(thethas.begin(), thethas.end());
        //     int m =  *min_element(thethas.begin(), thethas.end());
        //     x= (0.5*x)/(thethas.size());
        //     y= (0.5*y)/(thethas.size());
        cout<<"callback2=   "<<"M="<<M<<"  "<<"m="<<m<<endl;
        cout<<"callback2=   "<<"x="<<x<<"  "<<"y="<<y<<endl;
            if(M-m>5){
                int thetha = round(180-(M-m));
                cv::putText(frame,to_string(thetha),cv::Point(x,y+2),cv::FONT_HERSHEY_COMPLEX,1,cv::Scalar(255,0,0),1,cv::LINE_AA);

            }
            else{
                cv::putText(frame,"not found",cv::Point(x,y+2),cv::FONT_HERSHEY_COMPLEX,1,cv::Scalar(255,0,0),2);
                }
        
        try
        {
            if(!frame.empty()){
                cv::imshow("video_node3_1",frame);
                if(cv::waitKey(100)==27)
                {
                    cv::destroyAllWindows();
                }
            }
        }
        catch(cv_bridge::Exception& e)
        {
            ROS_ERROR("could not convert '%s' to 'bgr8'",msg1->encoding.c_str());
        }
        

    }




int main(int argc, char** argv){
    ros::init(argc, argv,"img_node3");
    
    ros::NodeHandle n3;
    cv::namedWindow("video_node3",cv::WINDOW_AUTOSIZE);
    image_transport::ImageTransport it(n3);
    image_transport::Subscriber sub = it.subscribe("video2",1,imgcallback);
    image_transport::Subscriber sub2 = it.subscribe("original_video",1,imgcallback2);
    ros::spin();
    cv::waitKey(100);

    cv::destroyAllWindows();
    return 0;
}


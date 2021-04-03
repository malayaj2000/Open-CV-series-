#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <sstream>
#include <bits/stdc++.h>

using namespace std;

//vector<vector<cv::Point> > contours;
vector<vector<cv::Point>> sortedcnts;
//vector<cv::Vec4i> hierarchy;
vector<cv::Vec3f> circles;
vector<cv::Vec3f> sortedcircles;
cv::Mat edge;
// bool compareContourAreas ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 ) {
//     double i = fabs( contourArea(cv::Mat(contour1)) );
//     double j = fabs( contourArea(cv::Mat(contour2)) );
//     return ( i > j );
// }

bool compareRadius(cv::Vec3f c1,cv::Vec3f c2){
    int r1 = c1[2];
    int r2 = c2[2];
    return (r1>r2);
}
void imageCallback(const sensor_msgs::ImageConstPtr& msg){
    cv::Mat frame;
    

    try
    {
        frame = cv_bridge::toCvShare(msg,"mono8")->image;

        
        if(!frame.empty())
        {
            cv::Canny(frame, edge,100,200);

            //cv::findContours(edge,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
            cv::HoughCircles(edge,circles,cv::HOUGH_GRADIENT,1,20,50,30,0,0);
            if(!circles.empty()){
                sort(circles.begin(),circles.end(),compareRadius);
            }
            int threshold = 2;
            sortedcircles.clear();
            for (int i = 0; i < threshold; i++)
            {
                /* code */
                sortedcircles.push_back(circles[i]);

            }
            
            // sort(contours.begin(),contours.end(),compareContourAreas);
            // int threshold = 2;
            // sortedcnts.clear();
            // for (int i = 0; i < threshold; i++)
            // {
            //     /* code */
            //     sortedcnts.push_back(contours[i]);

            // }
           
        }
        else
        {
            cv::destroyWindow("ball");
            return;
        }
        

    }
    catch(cv_bridge::Exception& e)
    {
        ROS_ERROR("unable to cvt '%s' to bgr8 ",msg->encoding.c_str());
    }
    
}


void imageCallback2(const sensor_msgs::ImageConstPtr& msg1){
    cv::Mat frame;
    frame = cv_bridge::toCvShare(msg1,"bgr8")->image;
    //  for (auto c:sortedcnts)
    //         {
    //             int epsilon  = 0.005 * cv::arcLength(c,true);
    //             vector<cv::Point> approx;
    //             vector<vector<cv::Point> > approxcnt;
    //             cv::approxPolyDP(c,approx,epsilon,true);
    //             approxcnt.push_back(approx);
    //             cv::drawContours(frame,approxcnt,0,cv::Scalar(255,255,84),2);
                
    //         }
    if(!circles.empty()){
        if(sortedcircles.size()==2 and abs(sortedcircles[0][0]-sortedcircles[1][0])<50){
            int x = (int)sortedcircles[0][0];
            int y = (int)sortedcircles[0][1];
            int r = (int)sortedcircles[0][2];
            cv::circle(frame,cv::Point(x,y),r,cv::Scalar(255,255,84),3);
            cv::circle(frame,cv::Point(x,y),1,cv::Scalar(0, 100, 100),3);
        }
        else{
            for (auto i:sortedcircles)
            {
                /* code */
                int x = (int)i[0];
                int y = (int)i[1];
                int r = (int)i[2];
                cv::circle(frame,cv::Point(x,y),r,cv::Scalar(255,255,84),3);
                cv::circle(frame,cv::Point(x,y),1,cv::Scalar(0, 100, 100),3);

            }
            
        }
    }
        try
        {
            if(!frame.empty()){
                cv::imshow("ball",frame);
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
int  main(int argc, char** argv)
{
    ros::init(argc, argv,"ball_node2");

    ros::NodeHandle nh;

    cv::namedWindow("ball",cv::WINDOW_AUTOSIZE);

    image_transport::ImageTransport  it(nh);

    image_transport::Subscriber sub1 = it.subscribe("video1",1,imageCallback);
    image_transport::Subscriber sub2 = it.subscribe("original",1,imageCallback2);

    ros::spin();

    cv::waitKey(100);

    cv::destroyAllWindows();

    return 0;
}
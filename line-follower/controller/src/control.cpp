#include<ros/ros.h>
#include<image_transport/image_transport.h>
#include<opencv2/highgui/highgui.hpp>
#include<cv_bridge/cv_bridge.h>
#include<opencv2/imgproc.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/opencv.hpp>
#include<controller/center_coordinate.h>
#include<bits/stdc++.h>

using namespace std;

cv::Mat frame;

cv::Mat birdView;

cv::Mat blur;

cv::Mat gray;

int th=37;

cv::Mat thres;

int width;

int height;

int wT=353;

int hT=707;

int wB=319;

int hB=769;

vector<cv::Point2f> prePoint;

vector<cv::Point2f> postPoint={{0,0},{800,0},{0,800},{800,800}};

vector<vector<cv::Point>> contours;

int frame_center_x = 0;
int frame_center_y = 0 ;

int path_center_x = 0 ;
int path_center_y  = 0;

ros::Publisher pub;

bool compareContourAreas(vector<cv::Point> c1,vector<cv::Point> c2){
    double i = fabs(cv::contourArea(c1));
    double j = fabs(cv::contourArea(c2));
    return i>j;
}

void callback(const sensor_msgs::ImageConstPtr& img_msg){
    
    try
    {
        /* code */
        frame = cv_bridge::toCvShare(img_msg,"bgr8")->image;

        width = frame.size().width;
        height = frame.size().height;
        frame_center_x = width/2;
        frame_center_y = height/2;

        postPoint.clear();
        postPoint.push_back(cv::Point2f(0,0));
        postPoint.push_back(cv::Point2f(width,0));
        postPoint.push_back(cv::Point2f(0,height));
        postPoint.push_back(cv::Point2f(width,height));


        prePoint.clear();
        prePoint.push_back(cv::Point2f(wT,hT));
        prePoint.push_back(cv::Point2f(width-wT,hT));
        prePoint.push_back(cv::Point2f(wB,hB));
        prePoint.push_back(cv::Point2f(width-wB,hB));
        
        cv::Mat Matrix = cv::getPerspectiveTransform(prePoint,postPoint);
        
        cv::warpPerspective(frame,birdView,Matrix,frame.size());
        
        for (int i = 0; i < 4; i++)
        {
            //cv::circle(frame,(int(prePoint[i][0]),int(prePoint[i][1])),15,(0,255,255),-1);
            //((int)prePoint[i].x,(int)prePoint[i].y)
            cv::circle(frame,cv::Point(prePoint[i].x,prePoint[i].y),15,(0,255,255),-1);
        }

        cv::GaussianBlur(birdView,blur,cv::Size(5,5),0,0);
        
        cv::cvtColor(blur,gray,cv::COLOR_BGR2GRAY);

        cv::threshold(gray,thres,th,255,cv::THRESH_BINARY_INV);

        
        
        cv::findContours(thres,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);

        sort(contours.begin(),contours.end(),compareContourAreas);

        for (auto c:contours)
        {
            int epsilon = 0.05 * cv::arcLength(c,true);
            vector<cv::Point> approx;
            vector<vector<cv::Point> > approxcnt;
            cv::approxPolyDP(c,approx,epsilon,true);
            //cout<<approx<<"ended"<<endl;
            cv::rectangle(birdView,cv::Point(approx[0].x,approx[0].y+50),cv::Point(approx[1].x,approx[1].y-50),cv::Scalar(0,255,0),2);
            approxcnt.push_back(approx);
            cv::drawContours(birdView,approxcnt,0,cv::Scalar(255,0,0),2);
            path_center_x = (approx[0].x + approx[1].x)/2;
            path_center_y = (approx[0].y+approx[1].y)/2;

            cv::circle(birdView,cv::Point(path_center_x,path_center_y),5,cv::Scalar(0,0,255),-1);


        }
        vector<cv::Moments> m(contours.size());
        for(auto c:contours){
            m[0] = cv::moments(c);
            int p_x = (int)m[0].m10 /m[0].m00;
            int p_y = (int)m[0].m10/m[0].m00;
            cv::circle(birdView,cv::Point(p_x,p_y),5,cv::Scalar(0,255,0),-1);
            //cout<<p_x <<"  "<<path_center_y<<endl;
        }
       
    
        cv::circle(birdView,cv::Point(frame_center_x,frame_center_y),5,cv::Scalar(255,0,0),-1);
        
        controller::center_coordinate center;
        center.frame_center_x = frame_center_x;
        center.frame_center_y = frame_center_y;
        center.path_center_x = path_center_x;
        center.path_center_y = path_center_y;

        // cout<<"frame_center_x = "<<frame_center_x<<endl;

        // cout<<"frame_center_y = "<<frame_center_y<<endl;
        
        // cout<<"path_center_x = "<<path_center_x<<endl;
        
        // cout<<"path_center_y = "<<path_center_y<<endl;

        pub.publish(center);

        if(!frame.empty()){
            cv::imshow("original",frame);
            cv::imshow("birdView",birdView);

            if(cv::waitKey(10) == 27){
                cv::destroyAllWindows();
            }
        }
    }
    catch(cv_bridge::Exception& e)
    {
        ROS_ERROR("could not convert '%s' to 'bgr8'",img_msg->encoding.c_str());
    }
    
    
}

int main(int argc,char** argv){
    ros::init(argc,argv,"control");


    ros::NodeHandle nh;


    // trackbar code (Temp)  
    cv::namedWindow("tracker",cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("threshold","tracker",&th,255);
    cv::createTrackbar("width top","tracker",&wT,800/2);
    cv::createTrackbar("hight top","tracker",&hT,800);
    cv::createTrackbar("width bottom","tracker",&wB,800/2);
    cv::createTrackbar("hight bottom","tracker",&hB,800);
    //cv::namedWindow("controller",cv::WINDOW_AUTOSIZE);



    image_transport::ImageTransport it(nh);

    

    image_transport::Subscriber sub = it.subscribe("/camera/image_raw",1,callback);


    pub = nh.advertise<controller::center_coordinate>("center",1);

    ros::Rate loop_rate(1);
    //ros::spinOnce();
    //loop_rate.sleep();

    cout<<"worked"<<endl;

    ros::spin();

    loop_rate.sleep();
    
    cv::waitKey(10);
    
    cv::destroyWindow("controller");
    
    return 0;
}

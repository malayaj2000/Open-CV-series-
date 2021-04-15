#include <ros/ros.h>
#include<controller/center_coordinate.h>
#include <geometry_msgs/Twist.h>
#include<bits/stdc++.h>

using namespace std;

int frame_center_x;

int frame_center_y;

int path_center_x;

int path_center_y;

int Kp = 0.01;
ros::Publisher pub;

geometry_msgs::Twist velocities;

int last_error =0;
int intg = 0;
int prop = 0;
int diff = 0;
float pid(int error){
    prop = error;
    intg = error+intg;
    diff = error-last_error;
    //cout<<"diff: "<<diff<<endl;
    float velocity = -0.004*error - 0.0001*diff ;

    last_error = error;
    cout<<velocity<<endl;
    return velocity;
}

void centerCb(const controller::center_coordinate::ConstPtr &center){

        // cout<<"frame_center_x = "<<center->frame_center_x<<endl;

        // cout<<"frame_center_y = "<<center->frame_center_y<<endl;
        
        // cout<<"path_center_x = "<<center->path_center_x<<endl;
        
        // cout<<"path_center_y = "<<center->path_center_y<<endl;

        frame_center_x = center->frame_center_x;

        frame_center_y = center->frame_center_y;
        
        path_center_x = center->path_center_x;
        
        path_center_y = center->path_center_y;

        

        velocities.linear.x = 0.2;
        int error = path_center_x - frame_center_x;
        //cout<<"error = "<<error<<endl;
        // velocities.angular.z = -error/100;
        velocities.angular.z = pid(error);

        pub.publish(velocities);

        
}

int main(int argc, char** argv){

    ros::init(argc,argv,"controllingRobo");

    ros::NodeHandle nh;

    pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel",1);

    ros::Subscriber sub1 = nh.subscribe("center",1,centerCb);
    
    ros::spin();
    if(!ros::ok()){
        velocities.linear.x = 0;
        velocities.angular.z = 0;
    }
    return 0;


}
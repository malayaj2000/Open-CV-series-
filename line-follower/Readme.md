## Line-Follower 
![opencv](https://img.shields.io/badge/OpenCV-cpp-green)
![ROS](https://img.shields.io/badge/ROS-melodic-red)
![Gazebo](https://img.shields.io/badge/Gazebo-simulator-lightgrey)
![CPP](https://img.shields.io/badge/CPP-g%2B%2B-blue)
![PID](https://img.shields.io/badge/PID-control-orange)

----------
### File Structure
  - Controller
    - src
      - control.cpp (image processing)
      - controlling_robo.cpp  (pid algo)
    - msg 
      - center_coordinate.msg 
 - line_follower (Gazebo Simulator) 


----------
### How to use it
  ```
  $ git clone https://github.com/malayaj2000/Open-CV-series-/tree/main/line-follower
  
  $ follow the steps inside line_follower file
  
  $ catkin_make
  
  $ rosrun controller control.cpp
  
  $ rosrun controller controlling_robo.cpp
  ```
----------
### Algorithm
  ```
   {
      INPUT : IMAGE RAW of Gazebo Simulator   
      
      1. Determining Ceter of the frame
      
        - center(frame_center_x,frame_center_y) = (frame.width/2 , frame.height/2)
        
        
      2. Determining Path Center 
      
          (A) Image Processing
            - perspective transform near the base using OpenCV.
            - Apply Thresholding to get the path segment.
            - Apply edge detection to get the clear edges.
            - Get Contour and Countor center. 
            - center(path_center_x,path_center_y)
            
      3.PID control 
      
          -linear_velocity.x = 0.2
          -error = path_center_x - frame_center_x
    
         pid(error)
            { 
                prop = error
                intg = error+intg
                diff = error-last_error
              
                velocity = -Kp*error - Kd*diff 
              
                last_error = error
              
                return velocity
            }
          
          angular_velocity.z = pid(error)
    
    OUTPUT : [linear_velocity(0.2,0,0) ,angular_velocity(0,0,pid(error))] 
    }
  ```
----------
## Results

![o/p video](https://github.com/malayaj2000/Open-CV-series-/blob/main/line-follower/line-follower.gif)

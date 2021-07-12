### Finding angle of a object present underwater with basic math and image prosessing in opencv
![cpp](https://img.shields.io/badge/Cpp-gcc%2017-blue)
![ROS](https://img.shields.io/badge/ROS-melodic-red)
![OpenCV](https://img.shields.io/badge/OpenCV-cpp-green)

-----------------

### Algorithm
```
{ 
    INPUT : Image 
    1. image processing 
        (A) Segment out the yellow part using opencv inRange function by converting the BGR image to HSV
        (B) Apply Blurring
        (C) detect edge using opencv canny edge detection
    2. Find line and the edge point of the line
        (A) Using Hough lines Find the line and their respective starting and ending point
        (B) Store it in a array = [[(x1,y1),(x2,y2)],...]
    3. Angle calculation
        - thethas =[]
        - for i = 1 to  array.size() :
            if x1-x2 == 0 : thetha.push_back(90°)
            
            else 
              th = atan((y2-y1)/(x2-x1))*(180/3.14)
              thetha.push_back(th)
        - m = min(thethas)
        - M = max(thethas)
        
        - angle = (180 - (M-m)) 
            
    OUTPUT: angle 

}
```
--------------------
  #### Results
![video](https://github.com/malayaj2000/Open-CV-series-/blob/main/UnderWater%20Angle%20Calculation/angle%20py%20.gif)

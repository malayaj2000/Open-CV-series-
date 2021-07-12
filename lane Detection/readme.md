# lane detection

![python](https://img.shields.io/badge/python-3.8-blue)
![opencv](https://img.shields.io/badge/OpenCV-python-green)

## Approch towards the lane detection
```
    1. convert frames to hsv color space .
    2. do preceptive transform for bird view of the lane.
    3. filter out yellow and white line separately using opencv .
    4. using bitwise_or on both the image to make a single mask containing both yellow and white line
    5. use the mask to get the lane.
 ```
  ### Results
![Result_video](https://github.com/malayaj2000/Open-CV-series-/blob/main/lane%20Detection/output%20.gif)



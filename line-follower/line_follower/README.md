# LINE FOLLOWER SIMULATION

## Prerequisites
- Ubuntu 16.04 or newer (Ubuntu 18.04 recommended)
- [ROS Kinetic ](http://wiki.ros.org/kinetic/Installation/Ubuntu) (Ubuntu 16.04) or [ROS Melodic ](http://wiki.ros.org/melodic/Installation/Ubuntu) (Ubuntu 16.04)
- [Catkin Tools](https://catkin-tools.readthedocs.io/en/latest/installing.html)


## Build
Build all the packages by running this inside your workspace
```sh
$ catkin build line_follower
$ source devel/setup.bash
```

## Extract model files
Extract models.zip to home/.gazebo/
```sh
$ roscd line_follower
$ unzip models.zip -d ~/.gazebo/models/
```

## Utilities
Teleop twist keyboard:
```sh
$ https://github.com/ros-teleop/teleop_twist_keyboard.git
```

## To launch the arena world in gazebo
Open terminal
```
$ source ~/.bashrc
$ roslaunch line_follower arena.launch

### To view all the active topics
make sure gazebo world is running

$ rostopic list

We would be only requiring to subscribe from the "/camera/image_raw" topic(after subscribing use cv_bridge to convert it into bgr8 format) and publish to '/cmd_vel'(in form of twist messages for bot to move) for completion of whole task(i.e to follow the track).
 
### Todos

 - Simulate a camera based line following robot. The robot should subscribe to camera/image_raw, perform image processing to detect the track/line, then use some control algorithms to align itself with the track.

License
----

MIT


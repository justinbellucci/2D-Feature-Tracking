# 2D Feature Tracking

<img src="images/keypoints.png" width="820" height="248" />

In preparation for building a collision detection system, this project tests various keypoint detector and descriptor combinations to see which one performs the best. Implemented in OpenCV the following keypoint deetectors were tested:  
* **Gradient-based detectors** -> Shi-Tomasi, Harris, SIFT
* **Binary detectors** -> FAST, BRISK, ORB, AKAZE, BRIEF

Along with the following descriptor extractors:
* **Gradient-based descriptors** -> SIFT
* **Binary descriptors** -> BRISK, ORB, AKAZE, BRIEF, FREAK

## Rubric Items Addressed 
_MP.1 Data Buffer Optimization_ - This is done by implenting a standard queue. Line 72 in MidTermProject_Camera_Student.cpp 

_MP.2 Keypoint Detection_ - Modern and classic detectors are implemented in matching2D_Student.cpp starting on line 116.  

_MP.3 Keypoint Removal_ - Keypoints are removed outside a pre-defined recatangle on line 104 in MidTermProject_Camera_Student.cpp  

_MP.4 Keypoint Descriptors_ - Keypoint descriptors are implemented in matching2D_Student.cpp starting on line 67.  

_MP.5 Descriptor Matching_ - FLANN and kNN selection methods implemented in matching2D_Student.cpp starting on line 7.  

_MP.6 Descriptor Distance Ratio_ - Filter keypoints implemented in matching2D_Student.cpp starting on line 50.



## Dependencies for Running Locally
The following dependencies are required to run the program locally.
* cmake >= 3.17
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.3 (Linux, Mac)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
* OpenCV >= 4.5
  * The OpenCV 4.5 source code can be found [here](https://github.com/opencv/opencv/tree/4.1.0)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
* NOTE: This project is tested using Mac OSX 10.15


## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./2D_feature_tracking`.
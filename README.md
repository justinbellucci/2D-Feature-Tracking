# 2D Feature Tracking

<img src="images/keypoints.png" width="820" height="248" />

In preparation for building a collision detection system, this project tests various keypoint detector and descriptor combinations to see which one performs the best. Implemented in OpenCV the following keypoint deetectors were tested:  
* **Gradient-based detectors** -> Shi-Tomasi, Harris, SIFT
* **Binary detectors** -> FAST, BRISK, ORB, AKAZE, BRIEF

Along with the following descriptor extractors:
* **Gradient-based descriptors** -> SIFT
* **Binary descriptors** -> BRISK, ORB, AKAZE, BRIEF, FREAK
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
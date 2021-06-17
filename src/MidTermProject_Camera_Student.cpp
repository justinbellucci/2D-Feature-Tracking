/* INCLUDES FOR THIS PROJECT */
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <memory>
#include <queue>
#include <limits>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

#include "dataStructures.h"
#include "matching2D.hpp"

using namespace std;

/* MAIN PROGRAM */
int main(int argc, const char *argv[])
{

    /* INIT VARIABLES AND DATA STRUCTURES */

    // data location
    string dataPath = "../";

    // camera
    string imgBasePath = dataPath + "images/";
    string imgPrefix = "KITTI/2011_09_26/image_00/data/000000"; // left camera, color
    string imgFileType = ".png";
    int imgStartIndex = 0; // first file index to load (assumes Lidar and camera names have identical naming convention)
    int imgEndIndex = 9;   // last file index to load
    int imgFillWidth = 4;  // no. of digits which make up the file index (e.g. img-0001.png)

    // misc
    int dataBufferSize = 2;       // no. of images which are held in memory (ring buffer) at the same time
    std::queue<DataFrame> dataBufferQ; // TODO:
    bool bVis = false;            // visualize results
    bool bFocusOnVehicle = true;
    cv::Rect vehicleRect(535, 180, 180, 150); // x, y, w, h
    bool bLimitKpts = false; // limit number of keypoints (helpful for debugging and learning)

    // keypoints and descriptors
    string detectorType = "HARRIS"; // Classic Detectors -> SHITOMASI, HARRIS
    // std::string detectorType = "BRISK"; // Modern Detectors -> FAST, BRISK, ORB, AKAZE, SIFT
    std::string descriptorType = "SIFT"; // BRISK, BRIEF, ORB, FREAK, AKAZE, SIFT

    /* MAIN LOOP OVER ALL IMAGES */

    for (size_t imgIndex = 0; imgIndex <= imgEndIndex - imgStartIndex; imgIndex++)
    {
        /* LOAD IMAGE INTO BUFFER */

        // assemble filenames for current index
        ostringstream imgNumber;
        imgNumber << setfill('0') << setw(imgFillWidth) << imgStartIndex + imgIndex;
        string imgFullFilename = imgBasePath + imgPrefix + imgNumber.str() + imgFileType;

        // load image from file and convert to grayscale
        cv::Mat img, imgGray;
        img = cv::imread(imgFullFilename);
        cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);

        // push image into data frame buffer
        std::unique_ptr<DataFrame> frame(new DataFrame);
        frame->cameraImg = imgGray;
        if(dataBufferQ.size() >= dataBufferSize)
        {
            dataBufferQ.pop();
        }

        dataBufferQ.emplace(*frame); // TODO: implment a buffer size of dataBufferSize to limit queue capacity

        std::cout << "---- DataBuffer Size = " << dataBufferQ.size() << std::endl;

        //// EOF STUDENT ASSIGNMENT
        cout << "#1 : LOAD IMAGE INTO BUFFER done" << endl;

        /* DETECT IMAGE KEYPOINTS */

        // extract 2D keypoints from current image
        vector<cv::KeyPoint> keypoints; // create empty feature list for current image
        // string detectorType = "HARRIS"; // Classic Detectors -> SHITOMASI, HARRIS
        // std::string detectorType = "FAST"; // Modern Detectors -> FAST, BRISK, ORB, AKAZE, SIFT

        if (detectorType.compare("SHITOMASI") == 0)
        {
            detKeypointsShiTomasi(keypoints, imgGray, bVis);
        }
        else if (detectorType.compare("HARRIS") == 0)
        {
            detKeypointsHarris(keypoints, imgGray, bVis);
        }
        else
        {
            detKeypointsModern(keypoints, imgGray, detectorType, bVis);
        }

        // only keep keypoints on the preceding vehicle
        if (bFocusOnVehicle)
        {
            std::vector<cv::KeyPoint> vehicleKeyPoints;
            for(auto it = keypoints.begin(); it < keypoints.end(); ++it)
            {
                if((it->pt.x >= vehicleRect.x && it->pt.x <= (vehicleRect.x + vehicleRect.width)) && 
                   (it->pt.y >= vehicleRect.y && it->pt.y <= (vehicleRect.y + vehicleRect.height)))
                {
                    vehicleKeyPoints.push_back(*it);
                }
            }
            keypoints = vehicleKeyPoints;
            std::cout << "     Vehicle keypoint count = " << vehicleKeyPoints.size() << std::endl;
        }
        
        if (bLimitKpts)
        {
            int maxKeypoints = 50;

            if (detectorType.compare("SHITOMASI") == 0)
            { // there is no response info, so keep the first 50 as they are sorted in descending quality order
                keypoints.erase(keypoints.begin() + maxKeypoints, keypoints.end());
            }
            cv::KeyPointsFilter::retainBest(keypoints, maxKeypoints);
            std::cout << "     NOTE: Keypoints have been limited!" << std::endl;
        }

        // push keypoints and descriptor for current frame to end of data buffer
        dataBufferQ.back().keypoints = keypoints;
        std::cout << "#2 : DETECT KEYPOINTS Done." << std::endl;

        /* EXTRACT KEYPOINT DESCRIPTORS */

        cv::Mat descriptors;
        // TODO: Add check for AKAZE keypoints when using AKAZE descriptor type
        descKeypoints(dataBufferQ.back().keypoints, dataBufferQ.back().cameraImg, descriptors, descriptorType);
       
        // push descriptors for current frame to end of data buffer
        dataBufferQ.back().descriptors = descriptors;
        cout << "#3 : EXTRACT DESCRIPTORS Done." << endl;

        if (dataBufferQ.size() > 1) // wait until at least two images have been processed
        {
            /* MATCH KEYPOINT DESCRIPTORS */
            bVis = true;
            vector<cv::DMatch> matches;
            string matcherType = "MAT_FLANN";        // MAT_BF, MAT_FLANN
            string descriptorType = "DES_BINARY"; // DES_BINARY, DES_HOG
            string selectorType = "SEL_KNN";       // SEL_NN, SEL_KNN
    
            matchDescriptors((dataBufferQ.front()).keypoints, (dataBufferQ.back()).keypoints,
                             (dataBufferQ.front()).descriptors, (dataBufferQ.back()).descriptors,
                             matches, descriptorType, matcherType, selectorType);

            // store matches in current data frame
            (dataBufferQ.back()).kptMatches = matches;
            cout << "#4 : MATCH KEYPOINT DESCRIPTORS done" << endl;

            // visualize matches between current and previous image
            bVis = true;
            if (bVis)
            {
                cv::Mat matchImg = ((dataBufferQ.back()).cameraImg).clone();
                cv::drawMatches((dataBufferQ.front()).cameraImg, (dataBufferQ.front()).keypoints,
                                (dataBufferQ.back()).cameraImg, (dataBufferQ.back()).keypoints,
                                matches, matchImg,
                                cv::Scalar::all(-1), cv::Scalar::all(-1),
                                vector<char>(), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

                string windowName = "Matching keypoints between two camera images";
                cv::namedWindow(windowName, 7);
                cv::imshow(windowName, matchImg);
                cout << "Press key to continue to next image" << endl;
                cv::waitKey(0); // wait for key to be pressed
            }
            bVis = false;
        }

    } // end of loop over all images

    return 0;
}
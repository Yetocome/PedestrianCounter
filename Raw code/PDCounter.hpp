//
//  PDCounter.hpp
//  PedestrianDetetion
//
//  Created by XMH on 1/31/16.
//  Copyright © 2016 XMH. All rights reserved.
//

#ifndef PDCounter_hpp
#define PDCounter_hpp

#include "Basis.hpp"
#include "PDDetector.hpp"
#include "Tracker.hpp"

#include <opencv2/opencv.hpp>
using namespace cv;

class PDCounter {
public:
    PDCounter(Mat& fframe);
    ~PDCounter();
    
    /* @Core */
    Mat detect(Mat& input); // The input mat should be sequential
    string stop(); // Stop recording: this function will clear cache in the class
    
    /* @Set */
    string adjust(int n); // Adjust the pareameter to accommodate user's environment
    string setDetector(int detector = 0); // Return value turns to the name of detector
    string clearArea(); // Clear the area setting
    bool addArea(Mat& frame); // Input the names of area, and this function
                              // will show an extra named window to let user
                              // to draw an new detecting area and test line
                              // if overlapped, return false and set fails
                              // you can use the bool result to show result
    bool delArea(Mat& frame, int ID = -1);
    bool delAreaByClick(Mat& frame);
    
    /* @Show Switch */
    string showPedestrianSwitch();
    string showTrajectorySwitch();
    string showAreasSwitch();
    // string showFpsSwitch();
    
    /* @Data */
    
    unsigned long getCurrNum(); // Return the number of pedestrians on the screen
    unsigned long getPosPastNum(int ID);
    unsigned long getNegPastNum(int ID);
    unsigned long getMultiPastNum(int fromID, int toID);
    unsigned long getGhostNum(int ID);
    unsigned long getLostNum();
    
    string getLog(); // Get the log of last operation
    
    /* @Custom */
    
    string trainReadMe(); // Return the help information of trainning
    // The trainning functins are optional only if the Detector is default(or set 0)
    // Otherwise, make the option gray
    // If time limits, you can choose not to add this function
    // The graphic trainning procedure is not necessary, I will try some ways to implement
    // this in the function, so you don't have to write too many extra codes
    bool trainBySelf(String& pic_file); // Choose the picture path to divide positive
    // samples and negative samples
    // Fails if the pictures don't suit
    bool trainBySelf(String& pos_file, String& neg_file); // Choose exiting samples' path
    
private:
    int areaID;
    int frameType;
    Mat colorMask;
    bool showPedestrian;
    bool showTrajectory;
    bool showArea;
    string lastLog;
    int PassedNum;
    int MaxPedestrianNum;
    vector<PDDetector> Detectors;
    PDTrackerList Trackers;
};

#endif /* PDCounter_hpp */


////////////////////////////Test Code////////////////////////////////////////
//VideoCapture cap(TEST_VIDEO_0);
////    VideoCapture cap;
////    cap.open("http://192.168.1.1:8080/?action=stream?dummy=param.mjpg");
//if (!cap.isOpened())
//return -1;
//
//Mat img, show_img;
//int lost_frame = 0;
//
//while (true) {
//    cap >> img;
//    if (img.data) {
//        break;
//    }
//}
//PDCounter pc(img);
////    pc.showPedestrianSwitch();
//pc.showTrajectorySwitch();
//pc.showAreasSwitch();
//namedWindow("Video", CV_WINDOW_AUTOSIZE);
//int start_frame = 0;
//int i = 0;
//
//while (true) { // Press any key to exit
//    /* code */
//    cap >> img;
//    if (i++  < start_frame) {
//        continue;
//    }
//    if (cvWaitKey(10) == 27)
//        break;
//    if (!img.data && lost_frame < 3) {
//        lost_frame++;
//        continue;
//    } else if (lost_frame >= 3) {
//        break;
//    } else {
//        lost_frame = 0;
//    }
//    show_img = pc.detect(img);
//    cout << pc.getLog() << endl;
//    cout << "Current Data:" << endl;
//    cout << "CurrNum:" << pc.getCurrNum() << endl; // Return the number of pedestrians on the screen
//    cout << "PosPastNum:" << pc.getPosPastNum(0) << endl;
//    cout << "NegPastNum:" << pc.getNegPastNum(0) << endl;
//    //        cout << "" << pc.getMultiPastNum(int fromID, int toID) << endl;
//    cout << "GhostNum:" << pc.getGhostNum(0) << endl;
//    cout << "LostNum:" << pc.getLostNum() << endl;
//    imshow("Video", show_img);
//    }





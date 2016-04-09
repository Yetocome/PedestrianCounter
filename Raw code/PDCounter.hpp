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
    bool delArea(int ID = -1); // Delete the area by ID(Not test)
    bool delAreaByClick(Mat& frame); // Delete the area by click(Not test)
    
    /* @Show Switch */
    string showPedestrianSwitch();
    string showTrajectorySwitch();
    string showAreasSwitch();
    // string showFpsSwitch();
    
    /* @Data */
    unsigned long getCurrNum(); // Return the number of pedestrians on the screen
    unsigned long getPosPastNum(int ID); // Return the number of pedestrians who passed the preset test line positively
    unsigned long getNegPastNum(int ID); // Return the number of pedestrians who passed the preset test line negatively
    unsigned long getMultiPastNum(int fromID, int toID);  // Return the number of pedestrians who passed from the fromID to the toID
    unsigned long getGhostNum(int ID); // Return the number of pedestrians who
    unsigned long getLostNum();  // Return the number of pedestrians who are lost while tracking
    unsigned long getTotalNum(); //Return the sum of all area
    unsigned long getAreaNum(); // Return the numbers of areas
    string getLog(); // Get the log of last operation
    
    /* @Custom */
    
    string trainReadMe(); // Return the help information of trainning
    // The trainning functins are optional only if the Detector is default(or set 0)
    // Otherwise, make the option gray
    // If time limits, you can choose not to add this function
    // The graphic trainning procedure is not necessary, I will try some ways to implement
    // this in the function, so you don't have to write too many extra codes
    void trainBySelf(String pic_file); // Choose the picture path to divide positive
    // samples and negative samples
    // Fails if the pictures don't suit
    void trainBySelf(String pos_file, String neg_file); // Choose exiting samples' path
    
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

class MySVM : public CvSVM
{
public:
    //获得SVM的决策函数中的alpha数组
    double * get_alpha_vector()
    {
        return this->decision_func->alpha;
    }
    
    //获得SVM的决策函数中的rho参数,即偏移量
    float get_rho()
    {
        return this->decision_func->rho;
    }
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
////    int width, height;
////    width = img.cols;
////    height = img.rows;
////    resize(img, show_img, Size(width*0.3, height*0.3));
////
//PDCounter pc(img);
//
////    pc.showPedestrianSwitch();
//pc.showTrajectorySwitch();
//pc.showAreasSwitch();
////    pc.adjust(1);
//namedWindow("Video", CV_WINDOW_AUTOSIZE);
//int start_frame = 0;
//int i = 0;
//
//while (true) { // Press any key to exit
//    /* code */
//    cap >> img;
//    //        resize(img, show_img, Size(width*0.3, height*0.3));
//    if (i++  < start_frame) {
//        continue;
//    }
//    char op = cvWaitKey(10);
//    if (op == 27) {
//        break;
//    }
//    else if (op == 'a') {
//        pc.addArea(img);
//    }
//    if (!img.data && lost_frame < 3) {
//        lost_frame++;
//        continue;
//    } else if (lost_frame >= 3) {
//        break;
//    } else {
//        lost_frame = 0;
//    }
//    show_img = pc.detect(img);
//    //        cout << pc.getLog() << endl;
//    
//    //
//    //        cout << "Current Data:" << endl;
//    //        cout << "CurrNum:" << pc.getCurrNum() << endl; // Return the number of pedestrians on the screen
//    //        cout << "PosPastNum:" << pc.getPosPastNum(0) << endl;
//    //        cout << "NegPastNum:" << pc.getNegPastNum(0) << endl;
//    //        //        cout << "" << pc.getMultiPastNum(int fromID, int toID) << endl;
//    //        cout << "GhostNum:" << pc.getGhostNum(0) << endl;
//    //        cout << "LostNum:" << pc.getLostNum() << endl;
//    if (pc.getAreaNum() > 1) {
//        int n = (int)pc.getAreaNum();
//        for (int i = 0 ; i < n; i++) {
//            for (int j = 0; j < n; j++) {
//                cout << "From " << i << " to " << j << ": " << pc.getMultiPastNum(i, j) << endl;
//            }
//        }
//    }
//    imshow("Video", show_img);
//}

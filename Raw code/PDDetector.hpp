//
//  PDDetector.hpp
//  PedestrianDetetion
//
//  Created by XMH on 2/22/16.
//  Copyright © 2016 XMH. All rights reserved.
//

#ifndef PDDetector_hpp
#define PDDetector_hpp

#include <iostream>
#include <opencv2/opencv.hpp>
#include "Basis.hpp"
#include "Tracker.hpp"
#include "PDClassifier.hpp"
#include "FGSeg.hpp"
#include "BlobFilter.hpp"
using namespace cv;

class PDDetector {
public:
    PDDetector(int id);
    PDDetector(Mat& frame, int id);
    
    unsigned long detect(Mat& frame, PDTrackerList& trackers);

    void showSwitch();
    void init(Mat& frame);
    void boom();

    
    string setDetector(int set = 0);
    void setScene(int set = 0);
    int getID();
    const Rect getArea(); // Output to check confilct.
    unsigned long getPosDirNum();
    unsigned long getNegDirNum();
    unsigned long getUnknownNum();
    unsigned long getMultiPastNum(int fromID);
private:
    /* Zone info */
    int Scene;
    Rect Area;
    int ID;
    Line testLine;
    Vec2d posDir;
    bool showPedestrian;
    /* Zone data */
    unsigned long posDirNum;
    unsigned long negDirNum;
    unsigned long unknownNum;
    unsigned long newNum;
    vector<unsigned long> elseNum;
    /* Core components */
    vector<Rect> tempRects;
    PDClassifier pdc;
    FGSeg fgs;
    BlobFilter bf;
};

#endif /* PDDetector_hpp */

///////////////////////Test Code/////////////////////////////////////////
//
//VideoCapture cap(TEST_VIDEO_0);
//if (!cap.isOpened())
//return -1;
//
//Mat img;
//int lost_frame = 0;
//
//
//PDDetector pdd(0);
//PDTrackerList pdt;
//cap >> img;
//while (true) {
//    if (img.data) {
//        pdd.init(img);
//        break;
//    }
//}
////    namedWindow("video raw", CV_WINDOW_AUTOSIZE);
//namedWindow("Video", CV_WINDOW_AUTOSIZE);
//pdd.showSwitch();
//int start_frame = 300;
//int i = 0;
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
//    
//    pdd.detect( img, pdt);
//    
//    imshow("Video", img);
//}
//


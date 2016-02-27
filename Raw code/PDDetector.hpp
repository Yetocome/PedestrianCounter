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
using namespace cv;



class PDDetector {
public:
    PDDetector(Mat& frame, int id);
    ~PDDetector();
    
    unsigned long detect(Mat& frame, PDTrackerList& trackers);

    void init();
    void boom();
    
    string setDetector(int set = 0);
    int getID();
    const Rect getArea(); // Output to check confilct.
    unsigned long getPosDirNum();
    unsigned long getNegDirNum();
    unsigned long getUnknownNum();
    unsigned long getMultiPastNum(int fromID);
private:
//    void initDetector(Mat& frame);
    Rect Area;
    int ID;
    Line testLine;
    Vec2d posDir;
    unsigned long posDirNum;
    unsigned long negDirNum;
    unsigned long unknownNum;
    unsigned long newNum;
    vector<unsigned long> elseNum;
    vector<Rect> tempRects;
    PDClassifier pdc;
    vector<int> buffer;
};

#endif /* PDDetector_hpp */



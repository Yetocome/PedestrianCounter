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
#include "PDClassifier.hpp"
#include "Tracker.hpp"

using namespace cv;

class PDDetector {
public:
    PDDetector(Mat& frame, int id);
    ~PDDetector();
    
    unsigned long detect(Mat& frame, PDTrackerList& trackers);
    const Rect getArea(); // Output to check confilct.
    void stop();
    string setDetector();
    void boom();
    
    unsigned long getPosDirNum();
    unsigned long getNegDirNum();
    unsigned long getUnknownNum();
    
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
    vector<int> elseNum;
    vector<Rect> tempRects;
    PDClassifier pdc;
    vector<int> buffer;
};

#endif /* PDDetector_hpp */

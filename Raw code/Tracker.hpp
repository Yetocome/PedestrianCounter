//
//  Tracker.hpp
//  PedestrianDetetion
//
//  Created by XMH on 1/31/16.
//  Copyright © 2016 XMH. All rights reserved.
//

#ifndef Tracker_hpp
#define Tracker_hpp

#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include "Basis.hpp"
#include "Utilities.hpp"

using namespace std;
using namespace cv;

class PDTrackerOne {
public:
    PDTrackerOne(Mat& frame, Rect trackBox, unsigned long AreaID, unsigned long PDID);

    /* Core */
    bool tracking(Mat& frame);
    PDSeq kill();
    bool lost(); // Boolean value false lasts 500 frames, return true.
    
    /* Set */
    string setAccuracy(unsigned int set = 0); // There are six subdivisions of matchTemplate method, from 0 to 5. Return the change log.
    string setTrappedCycle(int set = 500);
    double setSearchRange(double set = 3.0); // Must larger than 1, if set in a large number, the range will be the full image. Return the percentage of the input image.
    
    /* Get */
    const Pedestrian& getCurrPD();
    PDSeq& getTrajectory();
    int getDst();
    Vec2d getDir();

private:
    double calSearchWindow();
private:
    unsigned long who;
    Size size;
    PDSeq Trajectory;
    unsigned int chosenMethod;
    double searchRange;
    Rect searchWindow;
    int trappedTickClock; // Boolean value false it will, tacking stopped. Once true, becomes 500 again
    int defaultTTC;
    Size frameSize;
//    int Src; // Area ID
    int Dst; // Second cross place.
    Vec2d Dir = {0, 0};
};

class PDTrackerList {
public:
    PDTrackerList();
    ~PDTrackerList();
    
    PDTrackerOne& operator[](int ID);
    int tracking(Mat& frame);
//    int addTracker(Mat& fisrtFrame, Pedestrian& newcomer);
    unsigned long addTracker(Mat& fisrtFrame, Rect trackBox, int AreaID);

    PDSeq delTracker(int ID);
//    const PDSeq& getCurrPDSeq();
    const vector<Rect>& getCurrRects();
    unsigned long getSize();
//    const PDTrackerOne& getTracker(int ID);
    
    unsigned long boom();
    unsigned long getMutiPastNum(int fromID, int toID);
    unsigned long getLostNum();
private:
//    PDSeq currPD;
    unsigned long lostNum;
    unsigned long PDID;
    vector<Rect> currRects;
//    vector<PDTrackerOne*> trackers;
    vector<PDTrackerOne> trackers;
};

#endif /* Tracker_hpp */

////////////////////////////////Test Code/////////////////////////////////////////////

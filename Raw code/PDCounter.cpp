//
//  PDCounter.cpp
//  PDDetetion
//
//  Created by XMH on 1/31/16.
//  Copyright © 2016 XMH. All rights reserved.
//

#include "PDCounter.hpp"

//int frameType;
//Mat colorMask;
//bool showPedestrian;
//bool showTrajectory;
//bool showsArea;
//string lastLog;
//int PassedNum;
//int LostNum;
//int MaxPedestrianNum;
//vector<PDDetector> Detectors;
//PDTrackerList trackers;

PDCounter::PDCounter(Mat& fframe) {
    frameType = fframe.type();
    showPedestrian = true;
    showTrajectory = false;
    showArea = false;
    lastLog.assign("The main module begins running...");
    areaID = 0;
    PassedNum = 0;
    Detectors.push_back(PDDetector(fframe, 0));
    
}
PDCounter::~PDCounter() {
    
}

/* @Core */
Mat PDCounter::detect(Mat& input) {
    string log1("Begin detecting...\n");
    Mat output = input.clone();
    for (vector<PDDetector>::iterator it = Detectors.begin(); it != Detectors.end(); it++) {
        (*it).detect(input, Trackers);
    }
    string log2("All areas are detected...\n");
    
    Trackers.tracking(input); // NO parallel
    
    string log3("Tracking completed...\n");
    
    if (showPedestrian)
        drawFound(input, output, Trackers.getCurrRects());
    if (showArea) {
        for (vector<PDDetector>::iterator it = Detectors.begin(); it != Detectors.end(); it++) {
            drawArea(output, output, (*it).getArea());
        }
    }
    if (showTrajectory) {
        for (int i = 0; i < Trackers.getSize(); i++) {
            drawTrajectory(output, Trackers[i].getTrajectory());
        }
    }
    string log4("Drawing completed");
    lastLog = log1 + log2 + log3 + log4;
    return output;
}
string PDCounter::stop() {
    int num = (int)Trackers.boom();
    return "The main module was stopped, it has detected " + i_to_s(num) + " pedestrians in last run.";
}

/* @Set */
string PDCounter::adjust(int n) {
    switch (n) {
        case <#constant#>:
            <#statements#>
            break;
            
        default:
            break;
    }
}
string PDCounter::setDetector(int set) {
    string dname;
    for (vector<PDDetector>::iterator it = Detectors.begin(); it != Detectors.end(); it++) {
        dname = (*it).setDetector();
    }
    return "Successfully set all detectors to " + dname;
}
string PDCounter::initArea() {
    for (vector<PDDetector>::iterator it = Detectors.begin(); it != Detectors.end(); it++) {
        (*it).boom();
    }
    Detectors.clear();
    return "All the zones boom.";
}
bool PDCounter::addArea(Mat& frame) {
    Rect temp;
    PDDetector td(frame, areaID++);
    Rect newArea = td.getArea();
    for (vector<PDDetector>::iterator it = Detectors.begin(); it != Detectors.end(); it++) {
        temp = newArea & (*it).getArea();
        if (temp.area() > 0) {
            lastLog.assign("Overlap error. Please choose choose new zone to detect.");
            return false;
        }
    }
    lastLog.assign("Successfully created new zone to detect.");
    return true;
}
bool PDCounter::delArea(Mat& frame, int ID) {
    int counter = 0;
    for (vector<PDDetector>::iterator it = Detectors.begin(); it != Detectors.end(); it++, counter++) {
        if (ID == counter) {
            (*it).boom();
            Detectors.erase(it);
            lastLog.assign("Successfully delete the " + i_to_s(ID) + " zone.");
            return true;
        }
        
    }
    lastLog.assign("There is no zone identified with the ID " + i_to_s(ID));
    return false;
}


/* @Show Switch */
string PDCounter::showPedestrianSwitch() {
    showPedestrian = !showPedestrian;
    if (showPedestrian) {
        return "The state of showPedestrian is converted to True";
    }
    else
        return "The state of showPedestrian is converted to False";
}
string PDCounter::showTrajectorySwitch() {
    showTrajectory = !showTrajectory;
    if (showTrajectory) {
        return "The state of showPedestrian is converted to True";
    }
    else
        return "The state of showPedestrian is converted to False";
}
string PDCounter::showAreasSwitch() {
    showArea = !showArea;
    if (showArea) {
        return "The state of showPedestrian is converted to True";
    }
    else
        return "The state of showPedestrian is converted to False";
}
// string showFpsSwitch();

/* @Data */

unsigned long PDCounter::getCurrNum() {
    return Trackers.getSize();
}
unsigned long PDCounter::getPosPastNum(int ID) {
    return Detectors[ID].getPosDirNum();
}
unsigned long PDCounter::getNegPastNum(int ID) {
    return Detectors[ID].getNegDirNum();
}
unsigned long PDCounter::getMutiPastNum(int fromID, int toID) {
    return Trackers.getMutiPastNum(fromID, toID);
}
unsigned long PDCounter::getGhostNum(int ID) {
    return Detectors[ID].getUnknownNum();
}
unsigned long PDCounter::getLostNum() {
    return Trackers.getLostNum();
}
string PDCounter::getLog() {
    return lastLog;
}
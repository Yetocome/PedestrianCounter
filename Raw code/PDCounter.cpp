//
//  PDCounter.cpp
//  PDDetetion
//
//  Created by XMH on 1/31/16.
//  Copyright © 2016 XMH. All rights reserved.
//

#include "PDCounter.hpp"
#include "FGSeg.hpp"
#include "PDClassifier.hpp"
#include "Painter.hpp"
#include "Utilities.hpp"

#define MAX_AREA_NUM 4
#define PEDESTRIAN_COLOR Scalar(0, 255, 0)
#define AREA_COLOR Scalar(0, 255, 0)
#define Trajectory_COLOR Scalar(255, 0, 255)

PDCounter::PDCounter(Mat& fframe) {
    frameType = fframe.type();
    showPedestrian = true;
    showTrajectory = false;
    showArea = false;
    lastLog.assign("The main module begins running...");
    PassedNum = 0;
    Detectors.push_back(PDDetector(fframe, 0));
    areaID = 1;
    
}
PDCounter::~PDCounter() {
    
}

/* @Core */
Mat PDCounter::detect(Mat& input) {
    string log1("Begin detecting...\n");
    Mat output = input.clone();
    for (vector<PDDetector>::iterator it = Detectors.begin(); it != Detectors.end(); it++) {
        (*it).detect(output, Trackers);// NO parallel
    }
    string log2("All areas are detected...\n");
    
    string lost = i_to_s(Trackers.tracking(output)); // NO parallel
    
    string log3("Tracking completed...\nThe trackers lost " + lost + " pedestrians in this frame.\n");
    
    if (showPedestrian) {
        drawFounds(output, Trackers.getCurrRects(), PEDESTRIAN_COLOR);
    }
    if (showArea) {
        for (vector<PDDetector>::iterator it = Detectors.begin(); it != Detectors.end(); it++) {
            drawArea(output, (*it).getArea(), AREA_COLOR);
        }
    }
    if (showTrajectory) {
        for (int i = 0; i < Trackers.getSize(); i++) {
            drawTrajectory(output, Trackers[i].getTrajectory(), Trajectory_COLOR);
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
    for (vector<PDDetector>::iterator it = Detectors.begin(); it != Detectors.end(); it++) {
        (*it).setScene(n);
    }
    switch (n) {
        case 0:
            return "Successfully switch to the default scene";
            break;
        case 1:
            return "Successfully switch to the overlook scene";
        default:
            break;
    }
    return "";
}
string PDCounter::setDetector(int set) {
    string dname;
    for (vector<PDDetector>::iterator it = Detectors.begin(); it != Detectors.end(); it++) {
        dname = (*it).setDetector();
    }
    return "Successfully set all detectors to " + dname;
}
string PDCounter::clearArea() {
    for (vector<PDDetector>::iterator it = Detectors.begin(); it != Detectors.end(); it++) {
        (*it).boom();
    }
    Detectors.clear();
    return "All the zones boom.";
}
bool PDCounter::addArea(Mat& frame) {
    if (areaID > MAX_AREA_NUM) {
        lastLog.assign("Up to the limits. You cannot use zones more zhan " + i_to_s(MAX_AREA_NUM) + ". You can delete some zones to change your selection");
        return false;
    }
    
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
    Detectors.push_back(td);
    lastLog.assign("Successfully created new zone to detect.");
    return true;
}
bool PDCounter::delArea(Mat& frame, int ID) {
    for (vector<PDDetector>::iterator it = Detectors.begin(); it != Detectors.end(); it++) {
        if ((*it).getID() == ID) {
            (*it).boom();
            Detectors.erase(it);
//            if (it == Detectors.end()) {
//                break;
//            }
            lastLog.assign("Successfully delete the " + i_to_s(ID) + " zone.");
//            areaID--;
            return true;
        }
        
    }
    lastLog.assign("There is no zone identified with the ID " + i_to_s(ID));
    return false;
}

bool PDCounter::delAreaByClick(Mat& frame) {
    ////////////Pending to write./////////////////
    Mat show_frame;
    Point choose;
    for (vector<PDDetector>::iterator it = Detectors.begin(); it != Detectors.end(); it++) {
        drawArea(show_frame, show_frame, (*it).getArea(), AREA_COLOR);
        
    }
    pic_manipulator painter(show_frame);
    painter.get_click_info();
    choose = painter.getPoint();
    for (vector<PDDetector>::iterator it = Detectors.begin(); it != Detectors.end(); it++) {
        if (choose.inside((*it).getArea())) {
            int ID = (*it).getID();
            (*it).boom();
            Detectors.erase(it);
//            if (it == Detectors.end()) {
//                break;
//            }
            lastLog.assign("Successfully delete the " + i_to_s(ID) + " zone.");
            return true;
        }
    }
    lastLog.assign("Where you click is no zones.");
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
unsigned long PDCounter::getMultiPastNum(int fromID, int toID) {
    return Detectors[toID].getMultiPastNum(fromID);
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
//
//  Tracking.cpp
//  PedestrianDetetion
//
//  Created by XMH on 1/31/16.
//  Copyright © 2016 XMH. All rights reserved.
//

#include "Tracker.hpp"
#define Sim_Threshold 0.8

PDTrackerOne::PDTrackerOne(Mat& frame, Rect trackBox, unsigned long AreaID, unsigned long PDID) {
//    Trajectory[0].location = trackBox;
    Mat gray;
    cvtColor(frame, gray, CV_RGB2GRAY);
    
    who = PDID;
    size = trackBox.size();
    
    Pedestrian pd = {trackBox, gray(trackBox), PDID, AreaID};
    Trajectory.push_front(pd);
    
    chosenMethod = CV_TM_CCOEFF_NORMED;
    frameSize = frame.size();
    
    setSearchRange(3);
    defaultTTC = trappedTickClock = 500;

}

string PDTrackerOne::setAccuracy(unsigned int set) {
    chosenMethod = set;
    if (set > 5) {
        return "Wrong set, there is no change";
    }
    switch (set) {
        case 0:
            return "";
            break;
        
        default:
            return "";
            break;
    }
}
double PDTrackerOne::setSearchRange(double set) {
    searchRange = set;
    return calSearchWindow();
}
string PDTrackerOne::setTrappedCycle(int set) {
    defaultTTC = trappedTickClock = set;
    stringstream ss;
    ss << set;
    string log("Set the trapped test time to " + ss.str());
    log.append(" frames.");
    return log;
}

bool PDTrackerOne::tracking(Mat& frame) {
    Mat gray;
    cvtColor(frame, gray, CV_RGB2GRAY);
    calSearchWindow();
    
    Mat similarity;
    matchTemplate(gray(searchWindow), Trajectory[0].model, similarity, chosenMethod);
    imshow("Search window", gray(searchWindow));
//    normalize(similarity, similarity, 0, 1, NORM_MINMAX, -1, Mat());
    double minVal, maxVal;
    Point minLoc, maxLoc;
    Point matchLoc;
    
    minMaxLoc(similarity, &minVal, &maxVal, &minLoc, &maxLoc/*, Mat()*/);
    double ra = maxVal;
    
    if(chosenMethod  == CV_TM_SQDIFF || chosenMethod == CV_TM_SQDIFF_NORMED) {
        matchLoc = minLoc;
        ra = 1 - minVal;
    } else {
        matchLoc = maxLoc;
    }
    Rect location = {Point(matchLoc.x + searchWindow.x, matchLoc.y + searchWindow.y), size};
    Pedestrian pd = {location, gray(location), who};
    
    
    
    if (ra > Sim_Threshold) {
        trappedTickClock = defaultTTC;
        Trajectory.push_front(pd);
        if (Dir == Vec2d(0, 0)) {
            Point formerPoint(Trajectory[1].location.x, Trajectory[1].location.y);
            Point currPoint(Trajectory[0].location.x, Trajectory[0].location.y);
            Dir = calAngle(formerPoint, currPoint);
        }
        return true;
    }
    trappedTickClock--;

    return false;
    
}
PDSeq PDTrackerOne::kill() {
    
}
bool PDTrackerOne::lost() {
    if (trappedTickClock < 0) return true;
    return false;
}

const Pedestrian& PDTrackerOne::getCurrPD() {
    return Trajectory[0];
}
PDSeq& PDTrackerOne::getTrajectory() {
    return Trajectory;
}
int PDTrackerOne::getDst() {
    return Dst;
}
Vec2d PDTrackerOne::getDir() {
    return Dir;
}

double PDTrackerOne::calSearchWindow() {
    searchWindow.width = Trajectory[0].location.width * searchRange;
    searchWindow.height = Trajectory[0].location.height * searchRange;
    searchWindow.x = Trajectory[0].location.x + Trajectory[0].location.width * 0.5 - searchWindow.width * 0.5;
    searchWindow.y = Trajectory[0].location.y + Trajectory[0].location.height * 0.5 - searchWindow.height * 0.5;
    searchWindow &= Rect(Point(0,0), frameSize);
    
//    cout << "Search Range: " << endl;
//    cout << "Loc: " << "x-" << searchWindow.x << " y-" << searchWindow.y << endl;
//    cout << "Width: " << searchWindow.width << " Height: " << searchWindow.height << endl;
    
    return (double)searchWindow.width * searchWindow.height/
        (frameSize.width * frameSize.height);
}


//////////////////////////////////////////////////////////////

PDTrackerList::PDTrackerList() {
    PDID = 0;
}


PDTrackerOne& PDTrackerList::operator[](int ID) {
    return trackers[ID];
}

int PDTrackerList::tracking(Mat& frame) {
    int lost_per_frame = 0;
//    currPD.clear();
    currRects.clear();
    for (vector<PDTrackerOne>::iterator it = trackers.begin(); it != trackers.end(); it++) {
        if ((*it).tracking(frame))
            lost_per_frame++;
        if ((*it).lost()) {
            (*it).kill();
            trackers.erase(it);
        } else {
//            currPD.push_front((*it).getCurrPD());
            currRects.push_back((*it).getCurrPD().location);
        }
    }
    
    return lost_per_frame;
}
//int PDTrackerList::addTracker(Mat& fisrtFrame, Pedestrian& newcomer);
unsigned long PDTrackerList::addTracker(Mat& fisrtFrame, Rect trackBox, int AreaID) {
    unsigned long currSize = trackers.size();
    PDTrackerOne t(fisrtFrame, trackBox, AreaID, PDID++);
    trackers.push_back(t);
    return currSize;
}
PDSeq PDTrackerList::delTracker(int ID) {
    int counter = 0;
    PDSeq temp;
    for (vector<PDTrackerOne>::iterator it = trackers.begin(); it != trackers.end(); it++, counter++) {
        if (counter == ID) {
            temp = (*it).kill();
            trackers.erase(it);
        }
    }
    return temp;
}
//const PDSeq& PDTrackerList::getCurrPDSeq();
const vector<Rect>& PDTrackerList::getCurrRects() {
    return currRects;
}
unsigned long PDTrackerList::getSize() {
    return PDID;
}
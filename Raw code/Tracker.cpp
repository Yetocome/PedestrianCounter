//
//  Tracking.cpp
//  PedestrianDetetion
//
//  Created by XMH on 1/31/16.
//  Copyright © 2016 XMH. All rights reserved.
//

#include "Tracker.hpp"
#include "Utilities.hpp"
#include "Painter.hpp"
#include "Preserver.hpp"

Preserver TLogger("");

#define Sim_Threshold 0.8

PDTrackerOne::PDTrackerOne(Mat& frame, Rect trackBox, unsigned long AreaID, unsigned long PDID) {
//    Trajectory[0].location = trackBox;
    Mat gray;
    cvtColor(frame, gray, CV_RGB2GRAY);
    
    who = PDID;
    size = trackBox.size();
    lastLoc = {trackBox.x, trackBox.y};
    
    Src = (int)AreaID;
    Pedestrian pd = {trackBox, gray(trackBox), PDID, AreaID};
    Trajectory.push_front(pd);
    
    chosenMethod = CV_TM_CCOEFF_NORMED;
    frameSize = frame.size();
    
    setSearchRange(3);
    defaultTTC = trappedTickClock = 10; //Important parameter
    Dst = -1;

}

string PDTrackerOne::setAccuracy(unsigned int set) {
    if (set < 6) {
        chosenMethod = set;
    }
    switch (set) {
        case 0:
            return "CV_TM_SQDIFF";
            break;
        case 1:
            return "CV_TM_SQDIFF_NORMED";
            break;
        case 2:
            return "CV_TM_CCORR";
            break;
        case 3:
            return "CV_TM_CCORR_NORMED";
            break;
        case 4:
            return "CV_TM_CCOEFF";
            break;
        case 5:
            return "CV_TM_CCOEFF_NORMED";
            break;
        default:
            return "Wrong set, there is no change";
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
//    imshow("Search window", gray(searchWindow));
    
//    normalize(similarity, similarity, 0, 1, NORM_MINMAX, -1, Mat());
    double minVal, maxVal;
    Point minLoc, maxLoc;
    Point matchLoc;
    
    minMaxLoc(similarity, &minVal, &maxVal, &minLoc, &maxLoc/*, Mat()*/);
//    double ra = maxVal;
    
    if(chosenMethod  == CV_TM_SQDIFF || chosenMethod == CV_TM_SQDIFF_NORMED) {
        matchLoc = minLoc;
//        ra = 1 - minVal;
    } else {
        matchLoc = maxLoc;
    }
    Point currLoc(matchLoc.x + searchWindow.x, matchLoc.y + searchWindow.y);
    Rect location(currLoc, size);
    Pedestrian pd = {location, gray(location), who};
    
//    cout << "Now, the similarity is " << ra << endl;
//    if (ra > Sim_Threshold) {
    if (calDistanceSqr(currLoc, lastLoc) != 0) {
        trappedTickClock = defaultTTC;
        Trajectory.push_front(pd);
        int num = (int)Trajectory.getPDNum();
        if (num % 10 == 2 /*&& Dir == Vec2d(0, 0)*/) {

            Point formerPoint(Trajectory[num-1].location.x, Trajectory[num-1].location.y);
            Point currPoint;
            if (num>10)
                currPoint = Point(Trajectory[num-11].location.x, Trajectory[num-11].location.y);
            else
                currPoint = Point(Trajectory[0].location.x, Trajectory[0].location.y);
            Dir = calAngle(formerPoint, currPoint);
            cout << "ID: " << who << " Source: " << Src << " Destination: " << Dst << " Dir:<" << Dir[0] << ',' << Dir[1] << '>' << endl;
        }
//        cout << "True tracking: ";
//        cout << "Last loc: " << lastLoc.x << ',' << lastLoc.y << " turns to be " << currLoc.x << ',' << currLoc.y << endl;
        lastLoc = currLoc;
        return true;
    }
    trappedTickClock--;
//    cout << "Last loc: " << lastLoc.x << ',' << lastLoc.y << " turns to be " << currLoc.x << ',' << currLoc.y << endl;
    lastLoc = currLoc;
    return false;
    
}
string PDTrackerOne::kill() {
    string id = i_to_s((int)who);
    string lostLog("The pedestrian of ID " + id + " was lost.");
    string suicideLog("The pedestrian of ID " + id + " was killed deliberately.");
    TLogger.process("The log of pedestrian[" + id + "]:");
    TLogger.process(Trajectory);
    
    if (trappedTickClock < 0) {
        TLogger.process(lostLog);
        return lostLog;
    }
    TLogger.process(lostLog);
    return suicideLog;
}
bool PDTrackerOne::lost() {
    if (trappedTickClock < 0) return true;
    return false;
}

void PDTrackerOne::setDst(int set) {
    Dst = set;
}

const Pedestrian& PDTrackerOne::getCurrPD() {
    return Trajectory[0];
}
PDSeq& PDTrackerOne::getTrajectory() {
    return Trajectory;
}
int PDTrackerOne::getSrc() {
    return Src;
}
int PDTrackerOne::getDst() {
    return Dst;
}
Vec2d PDTrackerOne::getDir() {
    return Dir;
}

unsigned long PDTrackerOne::getID() {
    return who;
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
    lostNum = 0;
    showPDInfo = true;
}

PDTrackerList::~PDTrackerList() {
    
}


PDTrackerOne& PDTrackerList::operator[](int ID) {
    return trackers[ID];
}

int PDTrackerList::tracking(Mat& frame) {
    int lost_per_frame = 0;
//    currPD.clear();
    currRects.clear();
    for (vector<PDTrackerOne>::iterator it = trackers.begin(); it != trackers.end(); it++) {
        if (!(*it).tracking(frame))
            lost_per_frame++;
        if ((*it).lost()) {
            lostNum++;
            (*it).kill();
            trackers.erase(it);
            cout << "Successfully erased traker." << endl;
            if (it == trackers.end()) {
                break;
            }

        } else {
//            currPD.push_front((*it).getCurrPD());
            if (showPDInfo) {
                putPDInfo(frame, (*it).getCurrPD());
            }
            currRects.push_back((*it).getCurrPD().location);
            
        }
    }
    
    return lost_per_frame;
}

void PDTrackerList::setShowPDInfo(bool set) {
    showPDInfo = set;
}

//int PDTrackerList::addTracker(Mat& fisrtFrame, Pedestrian& newcomer);
unsigned long PDTrackerList::addTracker(Mat& fisrtFrame, Rect trackBox, int AreaID) {
    unsigned long currSize = trackers.size();
    PDTrackerOne t(fisrtFrame, trackBox, AreaID, PDID++);
    trackers.push_back(t);
    cout << "New traker established, the size now is " << trackers.size() << endl;
    return currSize;
}
string PDTrackerList::delTracker(int ID) {
    int counter = 0;
    string temp;
    for (vector<PDTrackerOne>::iterator it = trackers.begin(); it != trackers.end(); it++, counter++) {
        if (counter == ID) {
            temp.append((*it).kill());
            trackers.erase(it);
            if (it == trackers.end()) {
                break;
            }
        }
    }
    return temp;
}

int PDTrackerList::getOldID(Rect suspision, double similarity) {
    int ra = -1;
    double sim = 0.0;
    int i = 0;
    for (vector<PDTrackerOne>::iterator it = trackers.begin(); it != trackers.end(); it++, i++) {
        double tem = compareRect(suspision, (*it).getCurrPD().location);
        if (tem > sim) {
            sim = tem;
            ra = i;
        }
    }
    
    if (sim < similarity)
        ra = -1;
    return ra;
}

//const PDSeq& PDTrackerList::getCurrPDSeq();
const vector<Rect>& PDTrackerList::getCurrRects() {
    return currRects;
}
unsigned long PDTrackerList::getNum() {
    return PDID;
}

unsigned long PDTrackerList::getSize() {
    return trackers.size();
}


unsigned long PDTrackerList::boom() {
    unsigned long ra = PDID;
    for (int i = 0; i < PDID; i++) {
        delTracker(i);
    }
    PDID = 0;
    return ra;
}

unsigned long PDTrackerList::getLostNum() {
    return lostNum;
}


//
//  PDDetector.cpp
//  PedestrianDetetion
//
//  Created by XMH on 2/22/16.
//  Copyright © 2016 XMH. All rights reserved.
//

#include "PDDetector.hpp"
#include "Utilities.hpp"
#include "Painter.hpp"
#include "Preserver.hpp"
#define RECTANGLE_SIMILARITY 0.9

Preserver PDDLogger("");

PDDetector::PDDetector(int id) : ID(id) {
    
}

PDDetector::PDDetector(Mat& frame, int id) : ID(id) {
    init(frame);
}

unsigned long PDDetector::detect(Mat& frame, PDTrackerList& trackers) {
    Vec2d zeroVec = {0, 0};
    for (vector<int>::iterator it = noDirIDs.begin(); it != noDirIDs.end(); it++) {
        
        if (trackers[*it].getDir() == zeroVec) continue;
        if (dotProduct(trackers[*it].getDir(), posDir) > 0)
            posDirNum++;
        else {
            unknownNum++;
        }
        noDirIDs.erase(it); // Caution
        if (it == noDirIDs.end()) {
            break;
        }
    }
    
    tempRects = pdc.detect(frame(Area));
//    int i = 0;
//    cout << "Before: " << endl;
//    for (vector<Rect>::iterator it = tempRects.begin(); it != tempRects.end(); it++, i++) {
//        cout << i << ": "  << "Point: " << (*it).x << ',' << (*it).y << " Size: " << (*it).width << ',' << (*it).height << endl;
//    }
//    i = 0;
    modifyRects(tempRects, {Area.x, Area.y}, frame.size());
//    cout << "After: " << endl;
//    for (vector<Rect>::iterator it = tempRects.begin(); it != tempRects.end(); it++, i++) {
//        cout << i << ": " << "Point: " << (*it).x << ',' << (*it).y << " Size: " << (*it).width << ',' << (*it).height << endl;
//    }
    for (vector<Rect>::iterator it = tempRects.begin(); it != tempRects.end(); it++) {
        if (!intersectLineRect(testLine, *it)) {
            if (showPedestrian) {
                rectangle(frame, *it, Scalar(0, 0, 255));
            }

        } else {
            if (showPedestrian) {
                rectangle(frame, *it, Scalar(255, 0, 0));
            }
            int index = trackers.getOldID(*it, RECTANGLE_SIMILARITY);
            if (index < 0) { // It is a new pedestrian
                noDirIDs.push_back((int)trackers.addTracker(frame, (*it), ID));
//                buffer.push;
            } else { // It is an old pedestrian
                if (trackers[index].getSrc() != ID) { // Not duplicate
                    trackers[index].setDst(ID); // Only record the last area the pedestrian go through
                    if (dotProduct(trackers[index].getDir(), posDir) < 0) {
                        negDirNum++;
                    } else {
                        posDirNum++;
                    }
                }

            }
            
        }
        tempRects.erase(it);
        if (it == tempRects.end()) {
            break;
        }
    }
    newNum = noDirIDs.size();
    cout << "This frame has " << newNum << " new pedestrians." << endl;
    return newNum;
}

void PDDetector::showSwitch() {
    showPedestrian = !showPedestrian;
}

void PDDetector::init(Mat& frame) {
    pic_manipulator painter(frame);
    painter.draw_rect(true);
    Area = painter.getSelectRect();
    painter.draw_line(true);
    testLine = painter.getLine();
    posDirNum = 0;
    negDirNum = 0;
    unknownNum = 0;
    showPedestrian = false;
    testLine = getLineinRect(testLine, Area);
    posDir = calTestLineAngle(testLine);
}

void PDDetector::boom() {
    PDDLogger.process("Now the PDDetector of ID-" + i_to_s(ID) + " is logging:");
    PDDLogger.process("Area(x)", Area.x);
    PDDLogger.process("Area(y)", Area.y);
    PDDLogger.process("Area(width)", Area.width);
    PDDLogger.process("Area(height)", Area.height);
    PDDLogger.process("testLine(start.x)", testLine.begin.x);
    PDDLogger.process("testLine(start.y)", testLine.begin.y);
    PDDLogger.process("testLine(end.x)", testLine.end.x);
    PDDLogger.process("testLine(end.y)", testLine.end.x);
    PDDLogger.process("posDirNum", posDirNum);
    PDDLogger.process("negDirNum", negDirNum);
    PDDLogger.process("unknownNum", unknownNum);
    PDDLogger.process("newNum", newNum);
    PDDLogger.process("Else num:");
    for (int i = 0; i < elseNum.size(); i++) {
        PDDLogger.process("From area " + i_to_s(i), elseNum[i]);
    }
}


string PDDetector::setDetector(int set) {
    return pdc.setDetector(set);
}

int PDDetector::getID() {
    return ID;
}

const Rect PDDetector::getArea() {
    return Area;
}

unsigned long PDDetector::getMultiPastNum(int fromID) {
    return elseNum[fromID];
}

unsigned long PDDetector::getPosDirNum() {
    return posDirNum;
}
unsigned long PDDetector::getNegDirNum() {
    return negDirNum;
}
unsigned long PDDetector::getUnknownNum() {
    return unknownNum;
}

////////////////////////Test code///////////////////////////////
//
//

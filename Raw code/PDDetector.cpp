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

PDDetector::PDDetector(Mat& frame, int id) : ID(id) {
    pic_manipulator painter(frame);
    painter.draw_rect();
    Area = painter.getSelectRect();
    painter.show_rect();
    painter.draw_line();
    testLine = painter.getLine();
    posDirNum = 0;
    negDirNum = 0;
    unknownNum = 0;
    testLine = getLineinRect(testLine, Area);
    posDir = calTestLineAngle(testLine);
}
PDDetector::~PDDetector() {
    
}

unsigned long PDDetector::detect(Mat& frame, PDTrackerList& trackers) {
    Vec2d zeroVec = {0, 0};
    for (vector<int>::iterator it = buffer.begin(); it != buffer.end(); it++) {
        
        if (trackers[*it].getDir() == zeroVec) continue;
        if (dotProduct(trackers[*it].getDir(), posDir) > 0)
            posDirNum++;
        else {
            unknownNum++;
        }
        buffer.erase(it); // Caution
        if (it == buffer.end()) {
            break;
        }
    }
    
    tempRects = pdc.detect(frame(Area));
    for (vector<Rect>::iterator it = tempRects.begin(); it != tempRects.end(); it++) {
        if (!intersectLineRect(testLine, *it)) {
            tempRects.erase(it);
            if (it == tempRects.end()) {
                break;
            }
        } else {
            int index = trackers.getOldID(*it, RECTANGLE_SIMILARITY);
            if (index < 0) {
                buffer.push_back((int)trackers.addTracker(frame, (*it), ID));
//                buffer.push;
            } else {
                if (dotProduct(trackers[index].getDir(), posDir) < 0)
                    negDirNum++;
            }
        }
    }
    newNum = buffer.size();
//    vector<Rect> newRects;
//    newNum = checkNewRects(tempRects, trackers.getCurrRects(), newRects);
//    
//    for (vector<Rect>::iterator it = newRects.begin(); it != newRects.end(); it++) {
//        trackers.addTracker(frame, (*it), ID);
//    }
    return newNum;
}

void PDDetector::init() {
    posDirNum = 0;
    negDirNum = 0;
    unknownNum = 0;
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

//
//  PDDetector.cpp
//  PedestrianDetetion
//
//  Created by XMH on 2/22/16.
//  Copyright © 2016 XMH. All rights reserved.
//

#include "PDDetector.h"
#include "Utilities.h"
#include "Painter.h"
#include "Preserver.h"

#define RECTANGLE_SIMILARITY 0.6

Preserver PDDLogger("");

PDDetector::PDDetector(int id) : ID(id) {

}

PDDetector::PDDetector(Mat& frame, int id) : ID(id) {
    init(frame);
}

unsigned long PDDetector::detect(Mat& frame, PDTrackerList& trackers) {
//    Vec2d zeroVec = {0, 0};
    Vec2d zeroVec(0, 0);
    newNum = 0;

//    tempRects = pdc.detect(frame(Area));

//    fgs.setROI(frame(Area), Area);
    fgs.setROI(frame, Area);
//    tempRects = fgs.detect(frame);
    tempRects = pdc.detect(frame, fgs.detect(frame));

//    modifyRects(tempRects, {Area.x, Area.y}, frame.size());

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
                trackers.addTracker(frame, (*it), ID);
                newNum++;
            } else { // It is an unrecord pedestrian
                if (trackers[index].getTrajectory().getPDNum() == 2) {
                    if (dotProduct(trackers[index].getDir(), posDir) < 0) {
                        unknownNum++;
                    } else {
                        posDirNum++;
                    }
                } else { // It is an very old pedestrian
                    if (trackers[index].getSrc() != ID) { // Not duplicate
                        if (trackers[index].getDst() == -1) {
                            trackers[index].setDst(ID); // Only record the last area the pedestrian go through
                            if (dotProduct(trackers[index].getDir(), posDir) < 0) {
                                negDirNum++;
                            } else {
                                posDirNum++;
                            }
                        }
                    }
                }
            }

        }
        tempRects.erase(it);
        if (it == tempRects.end()) {
            break;
        }
    }
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

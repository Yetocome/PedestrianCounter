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

#define RECTANGLE_SIMILARITY 0.6

Preserver PDDLogger("");

PDDetector::PDDetector(int id) : ID(id) {
    elseNum.resize(id+1, 0);
}

PDDetector::PDDetector(Mat& frame, int id) : ID(id) {
//    elseNum.reserve(id);
//    elseNum.clear();
    elseNum.resize(id+1, 0);
    init(frame);
}

unsigned long PDDetector::detect(Mat& frame, PDTrackerList& trackers) {
//    Vec2d zeroVec = {0, 0};
    Vec2d zeroVec(0, 0);
    newNum = 0;
    
    switch (Scene) {
        case 0:
            fgs.setROI(frame, Area);
            tempRects = pdc.detect(frame, fgs.detect(frame));
            break;
        case 1:
//            Mat x= fgs.getMask(frame);
            Mat gray;
            cvtColor(frame,gray,CV_BGR2GRAY);
            tempRects= bf.filtrate(gray, Area);
            deweight(tempRects);
            break;
//        default:
//            break;
    }
//    modifyRects(tempRects, {Area.x, Area.y}, frame.size());

    for (vector<Rect>::iterator it = tempRects.begin(); it != tempRects.end(); it++) {
        if (!intersectLineRect(testLine, *it)) {
            if (showPedestrian) {
                rectangle(frame, *it, Scalar(0, 255, 0)); //If the pedestrians don't cross the test line, draw the green rectangles
            }

        } else {
            if (showPedestrian) {
                rectangle(frame, *it, Scalar(0, 255, 255)); //Yello rectangles
            }
            int index = trackers.getOldID(*it, RECTANGLE_SIMILARITY);
            if (index < 0) { // It is a new pedestrian
                trackers.addTracker(frame, (*it), ID);
                records.push_back(false);
                newNum++;
            } else { // It is an unrecord pedestrian
                if (trackers[index].getSrc() != ID) { // Distinguish from the first-entered zone
                    while (elseNum.size() < trackers[index].getSrc()+1) {
                        elseNum.push_back(0);
                    }
                    elseNum[trackers[index].getSrc()]++;
                    if (trackers[index].getDst() != ID) { // No repetition
                        trackers[index].setDst(ID); // Only record the last area the pedestrian go through
                        if (dotProduct(trackers[index].getDir(), posDir) < 0) {
                            negDirNum++;
                        } else {
                            posDirNum++;
                            cout << "From: " << trackers[index].getSrc() << "Posnum++ <" << trackers[index].getDir()[0] << "," << trackers[index].getDir()[1] << ">(Positive direction: <" << posDir[0] << "," << posDir[1] <<">)." << endl;
                        }
                    } else {
                       // Has been counted.
                    }
                } else {
                    if (records[trackers[index].getID()]) {
                        // do nothing
                    } else if (trackers[index].getDir() != Vec2d(0,0)) {
                        records[trackers[index].getID()] = true;
                        if (dotProduct(trackers[index].getDir(), posDir) < 0) {
                            unknownNum++;
                            cout << "Unknownum++ <" << trackers[index].getDir()[0] << "," << trackers[index].getDir()[1] << ">." << endl;
                        } else {
                            posDirNum++;
                            cout << "Posnum++ <" << trackers[index].getDir()[0] << "," << trackers[index].getDir()[1] << ">(Positive direction: <" << posDir[0] << "," << posDir[1] <<">)." << endl;
                        }
                    }
                }

//                if (trackers[index].getTrajectory().getPDNum() == 2) {
//                    if (records[index]) {
//                         // Do nothing
//                    } else if (dotProduct(trackers[index].getDir(), posDir) < 0) {
//                        unknownNum++;
//                    } else {
//                        posDirNum++;
//                    }
//                    records[index] = true;
//                } else { // It is an very old pedestrian
//                    if (trackers[index].getSrc() != ID) { // Not duplicate
//                        if (trackers[index].getDst() == -1) {
//                            trackers[index].setDst(ID); // Only record the last area the pedestrian go through
//                            if (dotProduct(trackers[index].getDir(), posDir) < 0) {
//                                negDirNum++;
//                            } else {
//                                posDirNum++;
//                            }
//                        }
//                    }
//                }
            }
            
        }
        tempRects.erase(it);
        if (it == tempRects.end()) {
            break;
        } // Avoid erasing the iterator which doesn't exist. 
    }
    
//    cout << "This frame has " << newNum << " new pedestrians." << endl;
    return newNum;
}

void PDDetector::showSwitch() {
    showPedestrian = !showPedestrian;
}

void PDDetector::init(Mat& frame) {
    pic_manipulator painter(frame);
    painter.draw_rect(true);
    Scene = 0;
    Area = painter.getSelectRect();
    painter.draw_line(true);
    testLine = painter.getLine();
    cout << "Test Line is settled, begin <" << testLine.begin.x << ',' << testLine.begin.y << ">, end <" << testLine.end.x << ',' << testLine.end.y << ">." << endl;
    posDirNum = 0;
    negDirNum = 0;
    unknownNum = 0;
    showPedestrian = false;
    testLine = getLineinRect(testLine, Area);
    posDir = calTestLineAngle(testLine);
    cout << "Direction is settled: <" << posDir[0] << ',' << posDir[1] << ">." << endl;
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

void PDDetector::newFriend() {
    elseNum.push_back(0);
}

string PDDetector::setDetector(int set) {
    return pdc.setDetector(set);
}

void PDDetector::setScene(int set) {
    Scene = set;
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

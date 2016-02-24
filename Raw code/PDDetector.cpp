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

#define RECTANGLE_SIMILARITY 0.9


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
    }
    
    tempRects = pdc.detect(frame(Area));
    for (vector<Rect>::iterator it = tempRects.begin(); it != tempRects.end(); it++) {
        if (!intersectLineRect(testLine, *it))
            tempRects.erase(it);
        else {
            int index = whichOldRect(*it, trackers.getCurrRects());
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


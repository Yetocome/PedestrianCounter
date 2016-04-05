//
//  Basis.cpp
//  PedestrianDetetion
//
//  Created by XMH on 2/22/16.
//  Copyright © 2016 XMH. All rights reserved.
//

#include "Basis.h"

PDSeq::PDSeq(int size) {
    seq.resize(size);
}
//PDSeq::~PDSeq() {
//
//}

Pedestrian& PDSeq::operator[](int ID) {
    return seq[ID];
}

//Pedestrian& PDSeq::getPD(int PDIndex) {
//    return seq[PDIndex];
//}
Pedestrian PDSeq::getPDByID(int PDID) {
    Pedestrian tem = {Rect(), Mat(), 0};
    for (deque<Pedestrian>::iterator it = seq.begin(); it != seq.end(); it++) {
        if ((*it).ID == PDID) tem = *it;
    }
    return tem;
}
Pedestrian PDSeq::delPD(int PDIndex) {
    Pedestrian tem = seq[PDIndex];
    int i = 0;
    for (deque<Pedestrian>::iterator it = seq.begin(); it != seq.end(); it++, i++) {
        if (i == PDIndex) {
            seq.erase(it);
            break;
        }
    }
    return tem;
}
Pedestrian PDSeq::delPDByID(int PDID) {
    Pedestrian tem = {Rect(), Mat(), 0};
    for (deque<Pedestrian>::iterator it = seq.begin(); it != seq.end(); it++) {
        if ((*it).ID == PDID) {
            tem = *it;
            seq.erase(it);
        }
    }
    return tem;
}

void PDSeq::clear() {
    seq.clear();
}
void PDSeq::push_front(Pedestrian p) {
    seq.push_front(p);
}
unsigned long PDSeq::getPDNum() {
    return seq.size();
}

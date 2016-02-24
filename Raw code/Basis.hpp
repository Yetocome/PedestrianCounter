//
//  Basis.hpp
//  PedestrianDetetion
//
//  Created by XMH on 2/22/16.
//  Copyright © 2016 XMH. All rights reserved.
//

#ifndef Basis_hpp
#define Basis_hpp

#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

struct Pedestrian {
    Rect location;
    Mat model;
    unsigned long ID;
    unsigned long AreaID;
};

struct Line {
    Point begin;
    Point end;
//    Vec2d angle;
};

class PDSeq {
public:
    PDSeq(int size = 0);
    ~PDSeq();
    
    Pedestrian& operator[](int ID);
//    Pedestrian& getPD(int PDIndex);
    Pedestrian getPDByID(int PDID);
    Pedestrian delPD(int PDIndex);
    Pedestrian delPDByID(int PDID);
    
    void clear();
    void push_front(Pedestrian p);
    unsigned long getPDNum();
private:
    deque<Pedestrian> seq;

};

#endif /* Basis_hpp */

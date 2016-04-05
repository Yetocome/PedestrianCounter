//
//  Preserver.hpp
//  PedestrianDetetion
//
//  Created by XMH on 2/25/16.
//  Copyright © 2016 XMH. All rights reserved.
//

#ifndef Preserver_h
#define Preserver_h

#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "Basis.h"

using namespace cv;
using namespace std;

class Preserver {
public:
    Preserver(string Name = "");
    ~Preserver();

    void setFilePath(string Path);
    void process(PDSeq& Dead);
    void process(string Record);
    void process(string Name, unsigned long Num);
private:
    string path;
    string file_name;
};


#endif /* Preserver_h */

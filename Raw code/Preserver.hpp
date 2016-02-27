//
//  Preserver.hpp
//  PedestrianDetetion
//
//  Created by XMH on 2/25/16.
//  Copyright © 2016 XMH. All rights reserved.
//

#ifndef Preserver_hpp
#define Preserver_hpp

#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "Basis.hpp"

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


#endif /* Preserver_hpp */

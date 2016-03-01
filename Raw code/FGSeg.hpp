//
//  FGSeg.hpp
//  PedestrianDetetion
//
//  Created by XMH on 1/31/16.
//  Copyright © 2016 XMH. All rights reserved.
//

#ifndef FGSeg_hpp
#define FGSeg_hpp

#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
using namespace std;
using namespace cv;

class FGSeg {
public:
    FGSeg();
    FGSeg(Rect t);
    ~FGSeg();
    
    bool setROI(Rect ROI);   // Set the Region of Interest in a picture
    bool setROI(vector<Rect>& ROIs);   // Set the Regions of Interest in a picture
    vector<Rect> detect(Mat& input); // The input mat should be sequential
    // Return the region of moving object in these picture
private:
    vector<Rect> re;
    int frame;
    Mat last;
    Rect total;
};

#endif /* FGSeg_hpp */

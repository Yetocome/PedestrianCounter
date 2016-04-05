#ifndef FGSeg_h
#define FGSeg_h

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;



class FGSeg {
public:
    FGSeg();
    ~FGSeg();
    bool setROI(Mat& img, Rect ROI);
    bool setROI(vector<Rect>& ROIs);
    vector<Rect> detect(Mat input);
    Mat getMask(Mat& input);
    Mat getGray(Mat& input);

private:
    vector<Rect> re;
    Rect total;
    int frame;
};

#endif

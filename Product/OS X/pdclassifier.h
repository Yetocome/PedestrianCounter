//
//  PedestrianClassifier.hpp
//  PedestrianDetetion
//
//  Created by XMH on 1/31/16.
//  Copyright © 2016 XMH. All rights reserved.
//

#ifndef PDClassifier_h
#define PDClassifier_h

#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>
using namespace std;
using namespace cv;

class PDClassifier {
public:
    PDClassifier(int set = 0);
    ~PDClassifier();

    void adjustAccuracy(unsigned int level); // The parameter of accuracy -- the smaller, the better
    const string setDetector(int set = 0);
//    vector<Rect> detect(Mat& img); // detect the full image to seek out pedestrians
    vector<Rect> detect(Mat img);
    vector<Rect> detect(Mat img, vector<Rect> ROIs);

private:
    static void init_info();
    HOGDescriptor hog;
    CascadeClassifier cas;
    vector<Rect> found;
    static vector<string> Detectors;
    int CurrDetector;
    double scaleFactor;
};

#endif /* PedestrianClassifier_h */



///////////////////////Test Code 1///////////////////////////
//VideoCapture cap(TEST_VIDEO_0);
//if (!cap.isOpened())
//return -1;
//
//Mat img;
//PDClassifier pc;
//namedWindow("video capture", CV_WINDOW_AUTOSIZE);
//pc.setDetector(0);
//
//vector<Rect> PDRects;
//while (waitKey(20) < 0) { // Press any key to exit
//    /* code */
//    cap >> img;
//    if (!img.data)
//        continue;
//    PDRects = pc.detect(img);
//    drawFounds(img, PDRects, Scalar(0, 255, 0));
//    imshow("video capture", img);
//}

///////////////////////Test Code 2///////////////////////////
//VideoCapture cap(TEST_VIDEO_0);
//if (!cap.isOpened())
//return -1;
//
//int zoneNum = 0;
//Mat img;
//PDClassifier pc;
//namedWindow("video capture", CV_WINDOW_AUTOSIZE);
//pc.setDetector(0);
//
//vector<Rect> PDRects, chosenZone;
//while (true) {
//    cap >> img;
//    if (img.data) {
//        break;
//    }
//}
//
//cout << "How many zones do you want to test?" << endl;
//cin >> zoneNum;
//pic_manipulator fimg(img);
//for (int i = 0; i < zoneNum; i++) {
//    fimg.draw_rect();
//    chosenZone.push_back(fimg.getSelectRect());
//}
//while (waitKey(20) < 0) { // Press any key to exit
//    /* code */
//    cap >> img;
//    if (!img.data)
//        continue;
//    PDRects = pc.detect(img, chosenZone);
//    drawFounds(img, PDRects, Scalar(0, 255, 0));
//    PDRects.clear();
//    imshow("video capture", img);
//}

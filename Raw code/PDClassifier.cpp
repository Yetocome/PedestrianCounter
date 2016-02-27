//
//  PedestrianClassifier.cpp
//  PedestrianDetetion
//
//  Created by XMH on 1/31/16.
//  Copyright © 2016 XMH. All rights reserved.
//

#include "PDClassifier.hpp"
#include "Utilities.hpp"

#define HaarCascadeFile "/Users/xmh/Desktop/PedestrianDetetion/PedestrianDetetion/Body/haarcascade_fullbody.xml"
vector<string> PDClassifier::Detectors(3);

PDClassifier::PDClassifier(int set) : CurrDetector(set){
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    cas.load(HaarCascadeFile);//
    scaleFactor = 1.1;
//    refNum = 0;
}

PDClassifier::~PDClassifier() {
    
}

void PDClassifier::init_info() {
    Detectors[0] = "Hog+SVM Detector(deault)";
    Detectors[1] = "Harr+Adaboost Detector";
    Detectors[2] = "Reserved Detector";
}

void PDClassifier::adjustAccuracy(unsigned int level) {
    scaleFactor = sqrt(level*0.1 + 1.01);
}

const string PDClassifier::setDetector(int set) {
    CurrDetector = set;
    return Detectors[set];
}

vector<Rect> PDClassifier::detect(Mat img) {
    switch (CurrDetector) {
        case 0:
            hog.detectMultiScale(img, found, 0, Size(8,8), Size(32,32), scaleFactor, 2);
            // img – Source image.
            // found_locations – Detected objects boundaries.
            // hit_threshold – Threshold for the distance between features and SVM classifying plane.
            // win_stride – Window stride. It must be a multiple of block stride.(8,8)
            // padding – Mock parameter to keep the CPU interface compatibility. It must be (0,0).
            // scale0 – Coefficient of the detection window increase.
            // group_threshold – Coefficient to regulate the similarity threshold. When detected, some objects can be covered by many rectangles. 0 means not to perform grouping. See groupRectangles() .

            break;
        case 1: {
            Mat gray;
            cvtColor(img, gray, CV_BGR2GRAY);
            equalizeHist(gray, gray);//直方图均衡化:通过拉伸像素强度分布范围来增强图像对比度
            cas.detectMultiScale(gray, found, scaleFactor, 2, 0|CV_HAAR_SCALE_IMAGE, Size(27, 27), Size(100,100));
            // image – Matrix of the type CV_8U containing an image where objects are detected.
            // objects – Vector of rectangles where each rectangle contains the detected object, the rectangles may be partially outside the original image.
            // numDetections – Vector of detection numbers for the corresponding objects. An object’s number of detections is the number of neighboring positively classified rectangles that were joined together to form the object.
            // scaleFactor – Parameter specifying how much the image size is reduced at each image scale.
            // minNeighbors – Parameter specifying how many neighbors each candidate rectangle should have to retain it.
            // flags – Parameter with the same meaning for an old cascade as in the function cvHaarDetectObjects. It is not used for a new cascade.
            // minSize – Minimum possible object size. Objects smaller than that are ignored.
            // maxSize – Maximum possible object size. Objects larger than that are ignored.
            // outputRejectLevels – Boolean. If True, it returns the rejectLevels and levelWeights. Default value is False.
            break;
        }
        default:
            break;
    }
    deweight(found);
    return found;
}
vector<Rect> PDClassifier::detect(Mat& img, vector<Rect>& ROIs) {
    vector<Rect> temp;
    switch (CurrDetector) {
        case 0:
            for (vector<Rect>::iterator iter= ROIs.begin(); iter != ROIs.end(); iter++) {
                hog.detectMultiScale(img(*iter), temp, 0, Size(8,8), Size(32,32), scaleFactor, 2);
                found.insert(found.end(), temp.begin(), temp.end());
                temp.clear();
            }

            break;
        case 1:
            for (vector<Rect>::iterator iter= ROIs.begin(); iter != ROIs.end(); iter++) {
                Mat gray;
                cvtColor(img, gray, CV_BGR2GRAY);
                equalizeHist(gray, gray);//直方图均衡化:通过拉伸像素强度分布范围来增强图像对比度
                cas.detectMultiScale(gray, temp, scaleFactor, 2, 0|CV_HAAR_SCALE_IMAGE, Size(27, 27), Size(100,100));
                found.insert(found.end(), temp.begin(), temp.end());
                temp.clear();
            }

            break;
        default:
            break;
    }
//    refResult();
    deweight(found);
    return found;
}

//int PDClassifier::refResult() {
//    int num = 0;
//
//    for (vector<Rect>::iterator i; i != found.end(); i++) {
////        std::cout << (*i).size() << endl;
//        Rect r(*i);
//        for (vector<Rect>::iterator j = i+1; j != found.end(); j++) {
//            if ((r & *j) == r) {
//                found.erase(i);
//                num++;
//            }
//        }
//    }
////    refNum += num;
//    return num;
//}




///////////////////////Test Code///////////////////////////
//
//    VideoCapture cap(TEST_VIDEO_0);
//    if (!cap.isOpened())
//        return -1;
//
//    Mat img, show_img;
//    PDClassifier pc;
//    namedWindow("video capture", CV_WINDOW_AUTOSIZE);
//    pc.setDetector(0);
//
//    while (waitKey(20) < 0) { // Press any key to exit
//       /* code */
//      cap >> img;
//      if (!img.data)
//        continue;
//      show_img = pc.detect(img);
//
//      imshow("video capture", show_img);
//    }




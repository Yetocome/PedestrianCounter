//
//  Utilities.hpp
//  PedestrianDetetion
//
//  Created by XMH on 2/22/16.
//  Copyright © 2016 XMH. All rights reserved.
//

#ifndef Utilities_hpp
#define Utilities_hpp

#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "Basis.hpp"

using namespace cv;
using namespace std;

/* Img&Others */

double compareImg(Mat& a, Mat& b);
void smoothTrajectory(PDSeq& list);
Mat rand_block(Mat &input);
Mat average_block(Mat &input);
void picture_add( Mat &frame , Mat &addition, Rect add_place );
void draw_dotted_line (Mat &src, Point start, Point end);
Mat mix_picture(Mat &a, Mat &b, int degree);
Mat mix_picture(Mat &a, Mat &b, Rect roi, int degree);
void picture_switch(Mat &last, Mat &next);

/* Rectangle */

double compareRect(Rect a, Rect b);
bool interRect(Rect a, Rect b); // Judge if two rects are intersection
int deweight(vector<Rect>& v);
int checkNewRects(vector<Rect>& temp, const vector<Rect>& oldRects, vector<Rect>& newRects);

/* Line&Vector&Point */

void rand_point(Point &in, Point start, Size rg1, Size rg2);
void rand_size(Size &in, Size max, double coef);
double dotProduct(Vec2d v1, Vec2d v2);
Vec2d calAngle(Point a, Point b);
int calDistanceSqr(Point a, Point b);
Vec2d calTestLineAngle(Line a);
int calCrossProduct(Line& l1, Line& l2);
double calP2LDistance(Point& p, Line& l);
bool ifLineCrossSeg(Line sLine, Line seg);
bool intersectLineRect(Line& sLine, Rect& area);
Line getLineinRect(Line& sLine, Rect& area);
Point intersectPoint(Line& a, Line& b);

/* Others */

string rand_string(int num, int density);
void become_doge(Mat &frame ,Rect doge_place, string path);
void become_doge( Mat &frame, Rect doge_place, Mat& doge);
string *generate_wordimage(Mat &src);
string i_to_s(int i);

template<typename T>
void rand_array(T a[], int n);

#endif /* Utilities_hpp */

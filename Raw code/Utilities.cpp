//
//  Utilities.cpp
//  PedestrianDetetion
//
//  Created by XMH on 2/22/16.
//  Copyright © 2016 XMH. All rights reserved.
//

#include "Utilities.hpp"
#include <cmath>
#include <ctime>

double compareImg(Mat& a, Mat& b) {
    double errorL2 = norm( a, b, CV_L2 );
    // Convert to a reasonable scale, since L2 error is summed across all pixels of the image.
    double similarity = errorL2 / (double)( a.rows * a.cols );
    return similarity;
}

double compareRect(Rect a, Rect b) {
    if (!interRect(a, b))
        return 0;
//    Rect cmp1, cmp2;
    if (a.area() > b.area()) {
        swap(a, b);
    }
    
    Rect re = a & b;
    return (double)re.area()/a.area();
}

bool interRect(const Rect a, const Rect b) {
    if ((a & b).size().area() > 0)
        return true;
    return false;
}

int deweight(vector<Rect>& v) {
    int num = 0;
    
    for (vector<Rect>::iterator i = v.begin(); i != v.end(); i++) {
        Rect r = *i;
        for (vector<Rect>::iterator j = i+1; j != v.end(); j++) {
            if ((r & (*j)) == r) {
                v.erase(i);
                num++;
                break;
            }
        }
        if (i == v.end()) {
            break;
        }
    }
    return num;
}

int calQuadrant(Vec2d v) {
    if (v[0] > 0) {
        if (v[1] > 0) {
            return 1;
        } else if (v[1] < 0) {
            return 4;
        } else {
            return 5;
        }
    } else if (v[0] < 0) {
        if (v[1] > 0) {
            return 2;
        } else if (v[1] < 0) {
            return 3;
        } else {
            return 7;
        }
    } else {
        if (v[1] > 0) {
            return 6;
        } else if (v[1] < 0) {
            return 8;
        } else {
            return 0;
        }
    }
}

Vec2d calTestLineAngle(Line a) {
    Vec2d temp = calAngle(a.begin, a.end);
    return {temp[1], -temp[0]};
}

Vec2d calAngle(Point a, Point b) {
    double x = b.x - a.x;
    double y = b.y - a.y;
    double hyp = sqrt(x*x + y*y);
//    cout << "a:(" << a.x << "," << a.y << ")" << endl;
//    cout << "b:(" << b.x << "," << b.y << ")" << endl;
//    cout << "hyp: " << hyp << endl;
    return {x/hyp, y/hyp};
}

int calDistanceSqr(Point a, Point b) {
    return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
}

int checkNewRects(vector<Rect>& temp, const vector<Rect>& oldRects, vector<Rect>& newRects);

void modifyRects(vector<Rect>& reRects, Point origin, Size limit) {
    Rect frame(Point(0, 0), limit);
    for (vector<Rect>::iterator it = reRects.begin(); it != reRects.end(); it++) {
        (*it).x += origin.x;
        (*it).y += origin.y;
        (*it) &= frame;
    }
}

int calCrossProduct(Line& l1, Line& l2) {
    Line sl1 = {Point(0,0), Point(l1.end.x-l1.begin.x, l1.end.y-l1.begin.y)};
    Line sl2 = {Point(0,0), Point(l2.end.x-l2.begin.x, l2.end.y-l2.begin.y)};
    
    return sl1.end.x*sl2.end.y - sl1.end.y*sl2.end.x;
}

double calP2PDistance(Point& a, Point& b) {
    return sqrt(pow(a.x-b.x, 2) + pow(a.y-b.y, 2));
}

double calP2LDistance(Point& p, Line& l) {
    double a, b, c;
    // 化简两点式为一般式
    // 两点式公式为(y - y1)/(x - x1) = (y2 - y1)/ (x2 - x1)
    // 化简为一般式为(y2 - y1)x + (x1 - x2)y + (x2y1 - x1y2) = 0
    // A = y2 - y1
    // B = x1 - x2
    // C = x2y1 - x1y2
    a = l.end.y - l.begin.y;
    b = l.begin.x - l.end.x;
    c = l.end.x * l.begin.y - l.begin.x * l.end.y;
    // 距离公式为d = |A*x0 + B*y0 + C|/√(A^2 + B^2)
    return abs(a * p.x + b * p.y + c) / sqrt(a * a + b * b);
}

bool ifLineCrossSeg(Line sLine, Line seg) {
    if (calCrossProduct(sLine, seg) == 0) return false;
    if (sLine.end.x == sLine.begin.x) {
        if ((seg.begin.x - sLine.begin.x)*(seg.end.x - sLine.begin.x) > 0)
            return false;
        else
            return true;
    }
    
    double k = (double)(sLine.end.y - sLine.begin.y)/(sLine.end.x - sLine.begin.x);
    double b = (double)(sLine.begin.x*sLine.end.y - sLine.begin.y*sLine.end.x)/(sLine.begin.x - sLine.end.x);
    
    
    //    if (calP2PDistance(seg.begin, seg.end) <
    //            max(calP2LDistance(seg.begin, sLine),
    //                calP2LDistance(seg.end, sLine)))
    if ((k*seg.begin.x+b-seg.begin.y)*(k*seg.end.x+b-seg.end.y) > 0)
        return false;
    return true;
}

bool intersectLineRect(Line& sLine, Rect& area) {
    Point A(area.x, area.y);
    Point B(area.x+area.width, area.y);
    Point C(area.x+area.width, area.y+area.height);
    Point D(area.x, area.y+area.height);
    
    return ifLineCrossSeg(sLine, {A, C}) || ifLineCrossSeg(sLine, {B, D});
}

Line getLineinRect(Line& sLine, Rect& area) {
    if (!intersectLineRect(sLine, area))
        return {Point(0,0), Point(0,0)};
    Point A(area.x, area.y);
    Point B(area.x+area.width, area.y);
    Point C(area.x+area.width, area.y+area.height);
    Point D(area.x, area.y+area.height);
    Line segAB = {A, B};
    Line segBC = {B, C};
    Line segCD = {C, D};
    Line segDA = {D, A};
    if (sLine.end.x == sLine.begin.x)
        return {{sLine.begin.x, A.y}, {sLine.begin.x, D.y}};
    double k = (double)(sLine.end.y - sLine.begin.y)/(sLine.end.x - sLine.begin.x);
    if (k == 0)
        return {{A.x, sLine.begin.y}, {B.x, sLine.begin.y}};
    
    double b = (double)(sLine.begin.x*sLine.end.y - sLine.begin.y*sLine.end.x)/(sLine.begin.x - sLine.end.x);
    
    vector<Point> cache;
    if (ifLineCrossSeg(sLine, segAB))
        cache.push_back(Point((A.y-b)/k, A.y));
    if (ifLineCrossSeg(sLine, segBC))
        cache.push_back(Point(B.x, k*B.x+b));
    if (ifLineCrossSeg(sLine, segCD))
        cache.push_back(Point((C.y-b)/k, C.y));
    if (ifLineCrossSeg(sLine, segDA))
        cache.push_back(Point(D.x, k*D.x+b));
    return {cache[0], cache[1]};
}

Point intersectPoint(Line& a, Line& b);

double dotProduct(Vec2d v1, Vec2d v2) {
    return v1[0]*v2[0] + v1[1]*v2[1];
}


////////////////////////////////////////////////////////////

/** 生成随机01 序列，可调整密度 **/
string rand_string(int num, int density){
    string a;
    for (int i = 0; i < num; i++) {
        int tem = rand();
        if (tem%3 == 0) a.push_back(' ');
        switch (density) {
            case 1:
                if (tem%3 == 1 && tem%5 == 0) a.push_back('1');
                else if (tem%3 == 2 && tem%5 == 0) a.push_back('0');
                break;
            case 2:
                if (tem%3 == 1 && (tem%5 == 0 || tem%5 == 1)) a.push_back('1');
                else if (tem%3 == 2 && (tem%5 == 0 || tem%5 == 1)) a.push_back('0');
                break;
            case 3:
                if (tem%3 == 1 && tem%5 != 0 && tem%5 != 1) a.push_back('1');
                else if (tem%3 == 2 && tem%5 != 0 && tem%5 != 1) a.push_back('0');
                break;
            case 4:
                if (tem%3 == 1 && tem%5 != 0) a.push_back('1');
                else if (tem%3 == 2 && tem%5 != 0) a.push_back('0');
                break;
            default:
                if (tem%3 == 1) a.push_back('1');
                else a.push_back('0');
                break;
        }
    }
    return a;
}

/** 随机生成点 **/
void rand_point(Point &in, Point start, Size rg1, Size rg2){
    //    srand((unsigned int)time(NULL));
    in.x = start.x+rand()%(rg1.width-rg2.width);
    in.y = start.y+rand()%(rg1.height-rg2.height);
}

/** 随机生成Size **/
void rand_size(Size &in, Size max, double coef){
    //    srand((unsigned int)time(NULL));
    int max_ = MAX(max.width, max.height);
    in.width = max_ - rand()%max_;
    in.height = in.width / coef;
}

/** doge **/
void become_doge( Mat &frame , Rect doge_place, string path){
    const Mat doge = imread(path);
    Mat doge_anysize;
    Mat imageROI = frame(doge_place);
    resize(doge, doge_anysize, doge_place.size());
    Mat mask;
    cvtColor(doge_anysize, mask, CV_BGR2GRAY);
    doge_anysize.copyTo(imageROI, mask);
}

void become_doge( Mat &frame, Rect doge_place, Mat& doge){
    Mat doge_anysize;
    Mat imageROI = frame(doge_place);
    resize(doge, doge_anysize, doge_place.size());
    Mat mask;
    cvtColor(doge_anysize, mask, CV_BGR2GRAY);
    doge_anysize.copyTo(imageROI, mask);
}

/** 生成图像string数组 **/
string *generate_wordimage(Mat &src){
    int n = src.rows;
    string *word_image = new string[n];
    Mat gray = src.clone();
    cvtColor(gray, gray, CV_BGR2GRAY);
    threshold(gray, gray, 120, 255, THRESH_BINARY);
    for (int y = 0; y < gray.rows; y++) {
        for (int x = 0; x < gray.cols; x++) {
            if (!gray.at<uchar>(y, x)) word_image[y].push_back('0');
            else word_image[y].push_back(' ');
        }
    }
    //    fstream output("/Users/xmh/Desktop/doge.txt", );
    ofstream output;
    output.open("/Users/xmh/Desktop/doge.txt");
    if (output.is_open()) {
        for (int i = 0; i < n; i++) {
            output << word_image[i];
        }
    }
    return word_image;
}

/** 将数字转换成string **/
string i_to_s(int i) {
    stringstream s;
    s << i;
    return s.str();
}

/** 打乱数组内元素算法 **/
template<typename T>
void rand_array(T a[], int n){
    int index, i;
    T tmp;
    srand((unsigned int)time(NULL));
    
    for(i=n-1; i>0; i--) {
        index=rand()%i;
        tmp=a[i];
        a[i]=a[index];
        a[index]=tmp;
    }
    
}

/** 图片分块内像素随机 **/
Mat rand_block(Mat &input){
    Mat output = input.clone();
    //    cvtColor(input, output, CV_BGR2GRAY);
    int n = output.cols*output.rows;
    int data[n];
    //    int i = n;
    //    while ((data[--i] = i)) ;
    for (int i = 0; i < n; i++) data[i] = i;
    rand_array(data, n);
    for (int y = 0, data_trace = 0; y < output.rows; y++) {
        for (int x = 0; x < output.cols; x++, data_trace++) {
            int x_ = data[data_trace]%output.cols;
            int y_ = data[data_trace]/output.cols;
            swap(output.at<Vec3b>(y, x), output.at<Vec3b>(y_, x_));
        }
    }
    //    cvtColor(output, output, CV_GRAY2BGR);
    return output;
}

/** 图片分块内像素平均 **/
Mat average_block(Mat &input){
    int n = input.cols*input.rows;
    int blue_sum = 0;
    int red_sum = 0;
    int yellow_sum = 0;
    for (int y = 0; y < input.rows; y++) {
        for (int x = 0; x < input.cols; x++) {
            blue_sum += input.at<Vec3b>(y, x)[0];
            red_sum += input.at<Vec3b>(y, x)[1];
            yellow_sum += input.at<Vec3b>(y, x)[2];
        }
    }
    Mat output(input.rows, input.cols, CV_8UC3, Scalar(blue_sum/n, red_sum/n, yellow_sum/n));
    return output;
}

/** 图像叠加 **/
void picture_add( Mat &frame , Mat &addition, Rect add_place ){
    Mat imageROI = frame(add_place);
    resize(addition, addition, add_place.size());
    Mat mask;
    cvtColor(addition, mask, CV_BGR2GRAY);
    addition.copyTo(imageROI, mask);
}

/** 画虚线 **/
void draw_dotted_line (Mat &src, Point start, Point end) {
    
}

/** 图像线性叠加，degree代表前一图片的程度从0~100的整数 **/
Mat mix_picture(Mat &a, Mat &b, int degree){
    Mat dst;
    double a_value = (double)degree/100, b_value = 1.0 - (double)degree/100;
    addWeighted(a, a_value, b, b_value, 0.0, dst);
    return dst;
}

//Mat mix_picture(Mat &a, Mat &b, Rect roi, int degree){
//    Mat dst;
//    double a_value = (double)degree/100, b_value = 1.0 - (double)degree/100;
//    addWeighted(a(roi), a_value, b(roi), b_value, 0.0, dst);
//    return dst;
//}

/** 图片的渐变切换 **/
void picture_switch(Mat &last, Mat &next){
    for (int i = 0; i < 100; i++){
        imshow(" ",mix_picture(next, last, i+1));
        cvWaitKey(20);
    }
}



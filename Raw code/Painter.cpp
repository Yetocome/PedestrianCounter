//
//  Painter.cpp
//  PedestrianDetetion
//
//  Created by XMH on 2/22/16.
//  Copyright © 2016 XMH. All rights reserved.
//

#include "Painter.hpp"
#include "Utilities.hpp"

const Mat doge = imread("/Users/xmh/Desktop/doge.jpg");
Mat doge_anysize;
double doge_coef = (double)doge.cols / doge.rows;

/** Part1 **/

pic_manipulator::pic_manipulator(){
    previousPoint = Point(-1, -1);
    
}
pic_manipulator::pic_manipulator(const string& a){
    src = imread(a);
    if (!src.data) {cout << "卧槽，打不开";}
    dst = src.clone();
    previousPoint = Point(-1, -1);
    greenMask = Mat(src.size(), CV_8UC4,Scalar(0, 255, 0));
}
pic_manipulator::pic_manipulator(Mat &a) : src(a){
    if (!src.data) {cout << "卧槽，打不开";}
    dst = src.clone();
    previousPoint = Point(-1, -1);
    greenMask = Mat(src.size(), CV_8UC4,Scalar(0, 255, 0));
}
pic_manipulator::~pic_manipulator(){
}

void pic_manipulator::load(const string &a){
    src = imread(a);
    if (!src.data) {cout << "卧槽，打不开";}
    dst = src.clone();
}

void pic_manipulator::load(Mat &pic){
    src = pic;
    dst = src.clone();
}

void pic_manipulator::switch_recover(bool flag) {
    if (flag) {
        re_dst = src;
    } else {
        re_dst = dst.clone();
    }
}

void pic_manipulator::recover(){
    dst = re_dst.clone();
}

void pic_manipulator::back_up(){
    if (backup.size() > backup_num) {
        backup.pop_front();
        backup.push_back(dst);
    }
    else
        backup.push_back(dst);
}

void pic_manipulator::change_backup_num(int n){
    backup_num = n;
}

void pic_manipulator::recover_last(int step){
    if (step > 0 && step <= backup.size()) {
        for (int i = 0; i < step-1; i++) backup.pop_back();
        dst = backup.back();
    }
}

void pic_manipulator::save_dst(){
    imwrite(save_path, dst);
}

/** Part2 **/
void pic_manipulator::picture_addscale(double degree){
    resize(dst, dst, Size(dst.cols*degree,dst.rows*degree));
}
void pic_manipulator::picture_reducescale(double degree){
    resize(dst, dst, Size(dst.cols*degree,dst.rows*degree));
}

void pic_manipulator::On_Mouth_Wipe(int event, int x, int y, int flag, void *param){
    pic_manipulator *pm = static_cast<pic_manipulator*>(param);
    
    string showname = pm->save_name;
    Point &origin = pm->previousPoint;
    Mat &showpic = pm->dst;
    int thick = pm->thickness;
    bool w_flag = pm->wipe_flag;
    
    imshow(showname, showpic);
    
    //判断松开左键或者不是左拖拽
    if ( event == EVENT_LBUTTONUP || !(flag & EVENT_FLAG_LBUTTON)) origin = Point(-1,-1);
    //鼠标左键按下信息
    else if (event == EVENT_LBUTTONDOWN) origin = Point(x,y);
    //鼠标左键按下并移动，进行擦除
    else if (event == EVENT_MOUSEMOVE && (flag & EVENT_FLAG_LBUTTON)) {
        Point pt(x,y);
        if (origin.x < 0) origin = pt;
        if (w_flag) line(showpic, origin, pt, Scalar::all(0), thick, 8, 0);
        else line(showpic, origin, pt, pm->line_color, thick, 8, 0);
        origin = pt;
    }
}
void pic_manipulator::picture_manual_wipe(int wf, int t, Scalar lc){
    wipe_flag = wf;
    thickness = t;
    line_color = lc;
    imshow(save_name, dst);
    setMouseCallback(save_name, On_Mouth_Wipe, this);
    while ( cvWaitKey() != 13 );
}

void pic_manipulator::On_Mouth_Drawrect(int event, int x, int y, int flag, void *param){
    pic_manipulator *pm = static_cast<pic_manipulator*>(param);
    
    bool &select_flag = pm->select;
    Rect &selection = pm->selection_rect;
    Point &origin = pm->rect_origin;
    Mat &image = pm->dst;
    string showname = pm->save_name;
    
    if(select_flag){
        selection.x = MIN(x, origin.x);//矩形左上角顶点坐标
        selection.y = MIN(y, origin.y);
        selection.width = abs(x - origin.x);//矩形宽
        selection.height = abs(y - origin.y);//矩形高
        
        selection &= Rect(0, 0, image.cols, image.rows);//确保所选的矩形区域在图片范围内
        pm->recover();
        //        Mat temp1 = image(selection);
        //        Mat temp2 = pm->greenMask(selection);
        //        Mat temp = mix_picture(temp1, temp2, 50);
        //        become_doge(image, selection, temp);
        rectangle(image, selection, Scalar(0, 255, 0), 2);
        
        string point_a_x = i_to_s(selection.x);
        string point_a_y = i_to_s(selection.y);
        string point_b_x = i_to_s(selection.x+selection.width);
        string point_b_y = i_to_s(selection.y+selection.height);
        pm->addtext('('+point_a_x+','+point_a_y+')', Point(selection.x, selection.y));
        pm->addtext('('+point_b_x+','+point_b_y+')', Point(selection.x+selection.width, selection.y+selection.height));
    }
    
    imshow(showname, image);
    
    switch(event){
        case EVENT_LBUTTONDOWN:
            //鼠标点下，选择区域
            origin = Point(x,y);
            selection = Rect(x,y,0,0);
            select_flag = true;
            break;
        case EVENT_LBUTTONUP:
            //鼠标松开，完成对区域的选择
            select_flag = false;
            break;
    }
}
void pic_manipulator::draw_rect(bool show){
    namedWindow(save_name);
    switch_recover(false);
    imshow(save_name, dst);
    setMouseCallback( save_name, On_Mouth_Drawrect, this );//设置鼠标回调函数，消息响应
    while (cvWaitKey() != 13 );
    destroyWindow(save_name);
    switch_recover();
    if (!show) {
        back_up();
        recover();
    }
}

void pic_manipulator::On_Mouth_Click(int event, int x, int y, int flag, void *param){
    pic_manipulator *pm = static_cast<pic_manipulator*>(param);
    Mat &image = pm->dst;
    
    switch (event) {
        case EVENT_LBUTTONDOWN:
            pm->point_place = Point(x, y);
            pm->point_blue = image.at<Vec3b>(y, x)[0];
            pm->point_green = image.at<Vec3b>(y, x)[1];
            pm->point_red = image.at<Vec3b>(y, x)[2];
            break;
        default:
            break;
    }
    
}
void pic_manipulator::get_click_info(){
    namedWindow(save_name);
    imshow(save_name, dst);
    setMouseCallback(save_name, On_Mouth_Click, this);
    cvWaitKey();
}

void pic_manipulator::show_info(){
    cout << "The point is on (" << point_place.x << ',' << point_place.y  << ')' << endl;
    cout << "The color is (" << point_blue << ',' << point_green << ',' << point_red << ')' << endl;
}
void pic_manipulator::show_and_change_rect(float angle){
    selection_rotated = RotatedRect(Point(selection_rect.x+selection_rect.width, selection_rect.y+selection_rect.height),
                                    Size(selection_rect.width, selection_rect.height), angle);
    back_up();
    
}

void pic_manipulator::On_Mouth_Line(int event, int x, int y, int flag, void *param) {
    pic_manipulator *pm = static_cast<pic_manipulator*>(param);
    
    if(pm->select){
        pm->selection_line.end.x = x;
        pm->selection_line.end.y = y;
        

        pm->recover();
        line(pm->dst, pm->selection_line.begin, pm->selection_line.end, Scalar(0, 0, 255));
        
        string p1 = '(' + i_to_s(pm->selection_line.begin.x) + ',' + i_to_s(pm->selection_line.begin.y) + ')';
        string p2 = '(' + i_to_s(pm->selection_line.end.x) + ',' + i_to_s(pm->selection_line.end.y) + ')';
        pm->change_font_color(Scalar(0, 0, 255));
        pm->addtext(p1, pm->selection_line.begin);
        pm->addtext(p2, pm->selection_line.end);
    }
    
    imshow(pm->save_name, pm->dst);
    
    
    switch (event) {
        case EVENT_LBUTTONDOWN:
            pm->selection_line.begin.x = x;
            pm->selection_line.begin.y = y;
            pm->select = true;
            break;
        case EVENT_LBUTTONUP:
            pm->select = false;
            break;
            
        default:
            break;
    }
}
void pic_manipulator::draw_line(bool show) {
    namedWindow(save_name);
    switch_recover(false);
    imshow(save_name, dst);
    setMouseCallback(save_name, On_Mouth_Line, this);
    while (cvWaitKey() != 13 );
    destroyWindow(save_name);
    switch_recover();
    if (!show) {
        back_up();
        recover();
    }
}

/** Part3 **/

void pic_manipulator::addtext(string text, Point place){
    putText(dst, text, place, font, font_scale, font_color, font_thickness, CV_AA);
}
void pic_manipulator::change_font(int cg){
    font = cg;
}
void pic_manipulator::change_font_thikness(int thick){
    font_thickness = thick;
}
void pic_manipulator::change_font_color(Scalar cg){
    font_color = cg;
}
void pic_manipulator::change_font_scale(double scale){
    font_scale = scale;
}

/** Part4 **/

//使用退出键，退出显示

void pic_manipulator::delete_watermark(int model){
    Mat mask = dst.clone();
    cvtColor(mask, mask, CV_BGR2GRAY);
    if (!model){
        for (int y = 0; y < mask.rows; y++) {
            for (int x = 0; x < mask.cols; x++) {
                if (!mask.at<uchar>(y,x)) {
                    mask.at<uchar>(y,x) = 255;
                }
                else mask.at<uchar>(y,x) = 0;
            }
        }
    }//at(y.x)函数返回(x,y)的坐标
    else if (model == 1){
        for (int y = 0; y < mask.rows; y++) {
            for (int x = 0; x < mask.cols; x++) {
                if (selection_rect.contains(Point(x, y))) {
                    mask.at<uchar>(y,x) = 255;
                }
                else mask.at<uchar>(y,x) = 0;
            }
        }
    }
    inpaint(src, mask, dst, 3, INPAINT_TELEA);
    //    inpaint(dst, mask, dst, 3, INPAINT_TELEA);
}

void pic_manipulator::picture_slice(){
    Mat tmp(dst, selection_rect);
    imwrite(save_path, tmp);
}

void pic_manipulator::change_save_path(string path){
    save_path = path;
}

void pic_manipulator::picture_mosaic(double grid_scale, int model){
    const int pix = 5*grid_scale;//固定的分块像素行/列数
    int m_rows = selection_rect.height / pix +1;
    int m_cols = selection_rect.width / pix +1;
    int origin_x = selection_rect.x;
    int origin_y = selection_rect.y;
    
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            Rect chosen(Point(origin_x+pix*j, origin_y+pix*i), Size(pix, pix));
            Mat roi = dst(chosen);
            if (model) roi = average_block(roi);
            else roi = rand_block(roi);
            picture_add(dst, roi, chosen);
        }
    }
    
}

Mat pic_manipulator::getDst() {
    return dst;
}

Rect pic_manipulator::getSelectRect() {
    return selection_rect;
}

Line pic_manipulator::getLine() {
    return selection_line;
}

Point pic_manipulator::getPoint() {
    return point_place;
}

void drawFound(Mat& input, Mat& output, vector<Rect> found, Scalar color) {
    input.copyTo(output); // Time consuming
    for (vector<Rect>::iterator iter = found.begin(); iter != found.end(); ++iter)
        rectangle(output, *iter, color, 2);
    
    
    
}

void drawFound(Mat& input, vector<Rect> found, Scalar color) {
    for (vector<Rect>::iterator iter = found.begin(); iter != found.end(); ++iter)
        rectangle(input, *iter, color, 2);
}
void putPDInfo(Mat& input, Mat& output, Pedestrian target) {
    
    
}

void drawTrajectory(Mat& img, PDSeq& list, Scalar color) {
    
    
}

void drawArea(Mat& input, Mat& output, Rect area, Scalar color) {
    
    
}


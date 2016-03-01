//
//  Painter.hpp
//  PedestrianDetetion
//
//  Created by XMH on 2/22/16.
//  Copyright © 2016 XMH. All rights reserved.
//

#ifndef Painter_hpp
#define Painter_hpp

#include <opencv2/opencv.hpp>
#include <vector>
#include "Basis.hpp"

using namespace std;
using namespace cv;

class pic_manipulator {
public:
    pic_manipulator();
    pic_manipulator(const string&);
    pic_manipulator(Mat&);
    ~pic_manipulator();
    
    /* 基本配置 */
    void load(const string& );//载入图片mark_f
    void load(Mat &pic);
    void switch_recover(bool flag = true); // 设置处理后的图像/(默认)原图像为还原图像
    void recover();//生成图片还原mark_a
    void back_up();//备份当前图像
    void change_backup_num(int n);//修改最大备份数量
    void recover_last(int step);//还原到上step步
    void save_dst();//保存生成图片mark_f
    
    /** degree是放大程度 eg:1.2 **/
    void picture_addscale(double degree);//等比例改变增大图片,mark_a
    void picture_reducescale(double degree);//等比例改变缩小图片,mark_a
    
    /* 鼠标操作配套函数 */
    void picture_manual_wipe(int wf = 1, int t = 1, Scalar lc = Scalar(0,255,0));//画彩色线条功能
    //默认用鼠标消除图片内容，修改wf为0则变成画线，t为粗细，lc为线的颜色，按回车键退出
    
    
    void get_click_info();
    void show_info();
    
    void draw_rect(bool show = 0);//mark_t
    void show_and_change_rect(float angle = 0);//弃用
    
    void draw_line(bool show = 0);
    /* 文字打印区 */
    void addtext(string text, Point place);//文本内容mark_a, mark_d
    void change_font(int cg);//mark_b
    /**
     FONT_HERSHEY_SIMPLEX = 0,
     FONT_HERSHEY_PLAIN =   1,
     FONT_HERSHEY_DUPLEX =  2,
     FONT_HERSHEY_COMPLEX = 3,
     FONT_HERSHEY_TRIPLEX = 4,
     FONT_HERSHEY_COMPLEX_SMALL =  5,
     FONT_HERSHEY_SCRIPT_SIMPLEX = 6,
     FONT_HERSHEY_SCRIPT_COMPLEX = 7,
     FONT_ITALIC = 16
     **/
    void change_font_thikness(int thick);//mark_b
    void change_font_color(Scalar cg);//mark_d
    void change_font_scale(double scale);//mark_d
    
    /* 其他功能区 */
    void delete_watermark(int model = 0);//去除水印函数（图像修复）,默认用wipe来修复,输入1来用矩阵修复,mark_b,mark_a
    void picture_slice();//图片切割函数，参数为保存路径，配合矩阵分割使用
    void change_save_path(string path);//统一保存图像的路径读入,mark_l
    void picture_mosaic(double grid_scale = 1.0, int model = 1);//在选定区域打码,grid_scale是格子的大小比例，model=1时均值马赛克，model=0时随机马赛克mark_a
    /* 获得数据区 */
    Mat getDst();
    Rect getSelectRect();
    Line getLine();
    Point getPoint();
    
private://info
    Point point_place;//mark_s.x mark_s.y
    int point_blue;
    int point_green;
    int point_red;
    Mat dst;
private:
    static void On_Mouth_Wipe(int event, int x, int y, int flag, void *param);
    static void On_Mouth_Click(int event, int x, int y, int flag, void *param);
    static void On_Mouth_Drawrect(int event, int x, int y, int flag, void *param);
    static void On_Mouth_Line(int event, int x, int y, int flag, void *param);
private://base_parameter
    Mat src;
    Mat re_dst;
    list<Mat> backup;
    int backup_num = 10;
    string save_name = "choose";
    string save_path = "重写";
private://manipulator_parameter
    int thickness;
    Point previousPoint;
    bool wipe_flag = 1;
    Scalar line_color = Scalar(0,255,0);
    
    bool select;
    Rect selection_rect;
    Line selection_line;
    RotatedRect selection_rotated;
    Point rect_origin;
private://font_parameter
    int font = FONT_HERSHEY_SIMPLEX;
    Scalar font_color = Scalar(0,255,0);
    int font_thickness = 1;
    double font_scale = 0.5;
    
private:
    Mat greenMask;
};

void drawFound(Mat& input, Mat& output, vector<Rect> found, Scalar color);
void drawFound(Mat& input, vector<Rect> found, Scalar color);
void putPDInfo(Mat& input, Mat& output, Pedestrian target);
void drawTrajectory(Mat& img, PDSeq& list, Scalar color = Scalar(255, 255, 0));
void drawArea(Mat& input, Mat& output, Rect area, Scalar color);


#endif /* Painter_hpp */

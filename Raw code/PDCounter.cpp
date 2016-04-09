//
//  PDCounter.cpp
//  PDDetetion
//
//  Created by XMH on 1/31/16.
//  Copyright © 2016 XMH. All rights reserved.
//

#include "PDCounter.hpp"
#include "FGSeg.hpp"
#include "PDClassifier.hpp"
#include "Painter.hpp"
#include "Utilities.hpp"
//#include <io.h>
#include <fstream>
#include <stdlib.h> //srand()和rand()函数

#define MAX_AREA_NUM 4
#define PEDESTRIAN_COLOR Scalar(0, 255, 0)
#define AREA_COLOR Scalar(0, 255, 0)
#define Trajectory_COLOR Scalar(255, 0, 255)

PDCounter::PDCounter(Mat& fframe) {
    frameType = fframe.type();
    showPedestrian = true;
    showTrajectory = false;
    showArea = false;
    lastLog.assign("The main module begins running...");
    PassedNum = 0;
    Detectors.push_back(PDDetector(fframe, 0));
    areaID = 1;
}
PDCounter::~PDCounter() {
    
}

/* @Core */
Mat PDCounter::detect(Mat& input) {
    string log1("Begin detecting...\n");
    Mat output = input.clone();
    for (vector<PDDetector>::iterator it = Detectors.begin(); it != Detectors.end(); it++) {
        (*it).detect(output, Trackers);// NO parallel
    }
    string log2("All areas are detected...\n");
    
    
    

    string lost = i_to_s(Trackers.tracking(output)); // NO parallel
    
    string log3("Tracking completed...\nThe trackers lost " + lost + " pedestrians in this frame.\n");

    if (showPedestrian) {
        drawFounds(output, Trackers.getCurrRects(), PEDESTRIAN_COLOR);
    }
    
    if (showArea) {
        for (vector<PDDetector>::iterator it = Detectors.begin(); it != Detectors.end(); it++) {
            drawArea(output, (*it).getArea(), AREA_COLOR);
            pic_manipulator namer(output);
            namer.change_font_color(Scalar(255, 255, 0));
            namer.addtext("Zone ID:" + i_to_s((*it).getID()), Point((*it).getArea().x, (*it).getArea().y));
            namer.addtext("Pos:" + i_to_s((int)(*it).getPosDirNum()), Point((*it).getArea().x, (*it).getArea().y+10));
            namer.addtext("Neg:" + i_to_s((int)((*it).getNegDirNum()+(*it).getUnknownNum())), Point((*it).getArea().x, (*it).getArea().y+20));
            output = namer.getDst();
        }
    }
    if (showTrajectory) {
        for (int i = 0; i < Trackers.getSize(); i++) {
            drawTrajectory(output, Trackers[i].getTrajectory(), Trajectory_COLOR);
        }
    }
    string log4("Drawing completed");
    lastLog = log1 + log2 + log3 + log4;
    return output;
}
string PDCounter::stop() {
    int num = (int)Trackers.boom();
    return "The main module was stopped, it has detected " + i_to_s(num) + " pedestrians in last run.";
}

/* @Set */
string PDCounter::adjust(int n) {
    for (vector<PDDetector>::iterator it = Detectors.begin(); it != Detectors.end(); it++) {
        (*it).setScene(n);
    }
    switch (n) {
        case 0:
            return "Successfully switch to the default scene";
            break;
        case 1:
            return "Successfully switch to the overlook scene";
        default:
            break;
    }
    return "";
}
string PDCounter::setDetector(int set) {
    string dname;
    for (vector<PDDetector>::iterator it = Detectors.begin(); it != Detectors.end(); it++) {
        dname = (*it).setDetector();
    }
    return "Successfully set all detectors to " + dname;
}
string PDCounter::clearArea() {
    for (vector<PDDetector>::iterator it = Detectors.begin(); it != Detectors.end(); it++) {
        (*it).boom();
    }
    Detectors.clear();
    return "All the zones boom.";
}
bool PDCounter::addArea(Mat& frame) {
    if (areaID > MAX_AREA_NUM) {
        lastLog.assign("Up to the limits. You cannot use zones more zhan " + i_to_s(MAX_AREA_NUM) + ". You can delete some zones to change your selection");
        return false;
    }
    
    Rect temp;
    PDDetector td(frame, areaID++);
    Rect newArea = td.getArea();
    for (vector<PDDetector>::iterator it = Detectors.begin(); it != Detectors.end(); it++) {
        temp = newArea & (*it).getArea();
        if (temp.area() > 0) {
            lastLog.assign("Overlap error. Please choose choose new zone to detect.");
            return false;
        }
    }
    for (vector<PDDetector>::iterator it = Detectors.begin(); it != Detectors.end(); it++) {
        (*it).newFriend();
    }
    Detectors.push_back(td);
    lastLog.assign("Successfully created new zone to detect.");
    return true;
}
bool PDCounter::delArea(int ID) {
    for (vector<PDDetector>::iterator it = Detectors.begin(); it != Detectors.end(); it++) {
        if ((*it).getID() == ID) {
            (*it).boom();
            Detectors.erase(it);
//            if (it == Detectors.end()) {
//                break;
//            }
            lastLog.assign("Successfully delete the " + i_to_s(ID) + " zone.");
//            areaID--;
            return true;
        }
        
    }
    lastLog.assign("There is no zone identified with the ID " + i_to_s(ID));
    return false;
}

bool PDCounter::delAreaByClick(Mat& frame) {
    ////////////Pending to write./////////////////
    Mat show_frame;
    Point choose;
    for (vector<PDDetector>::iterator it = Detectors.begin(); it != Detectors.end(); it++) {
        drawArea(show_frame, show_frame, (*it).getArea(), AREA_COLOR);
        
    }
    pic_manipulator painter(show_frame);
    painter.get_click_info();
    choose = painter.getPoint();
    for (vector<PDDetector>::iterator it = Detectors.begin(); it != Detectors.end(); it++) {
        if (choose.inside((*it).getArea())) {
            int ID = (*it).getID();
            (*it).boom();
            Detectors.erase(it);
//            if (it == Detectors.end()) {
//                break;
//            }
            lastLog.assign("Successfully delete the " + i_to_s(ID) + " zone.");
            return true;
        }
    }
    lastLog.assign("Where you click is no zones.");
    return false;
}


/* @Show Switch */
string PDCounter::showPedestrianSwitch() {
    showPedestrian = !showPedestrian;
    Trackers.setShowPDInfo(showPedestrian); // The default setting is ture;
    if (showPedestrian) {
        return "The state of showPedestrian is converted to True";
    }
    else
        return "The state of showPedestrian is converted to False";
}
string PDCounter::showTrajectorySwitch() {
    showTrajectory = !showTrajectory;
    if (showTrajectory) {
        return "The state of showPedestrian is converted to True";
    }
    else
        return "The state of showPedestrian is converted to False";
}
string PDCounter::showAreasSwitch() {
    showArea = !showArea;
    if (showArea) {
        return "The state of showPedestrian is converted to True";
    }
    else
        return "The state of showPedestrian is converted to False";
}
// string showFpsSwitch();

/* @Data */

unsigned long PDCounter::getCurrNum() {
    return Trackers.getSize();
}
unsigned long PDCounter::getPosPastNum(int ID) {
    return Detectors[ID].getPosDirNum();
}
unsigned long PDCounter::getNegPastNum(int ID) {
    return Detectors[ID].getNegDirNum();
}
unsigned long PDCounter::getMultiPastNum(int fromID, int toID) {
    return Detectors[toID].getMultiPastNum(fromID);
}
unsigned long PDCounter::getGhostNum(int ID) {
    return Detectors[ID].getUnknownNum();
}
unsigned long PDCounter::getLostNum() {
    return Trackers.getLostNum();
}

unsigned long PDCounter::getTotalNum() {
    return Trackers.getNum();
}

unsigned long PDCounter::getAreaNum() {
    return Detectors.size();
}

string PDCounter::getLog() {
    return lastLog;
}


CvPoint pt;
Mat img,tmp;
void on_mouse(int event,int x,int y,int flags,void *ustc)//event鼠标事件代号，x,y鼠标坐标，flags拖拽和键盘操作的代号
{
    static Point pre_pt = {-1,-1};//初始坐标
    static Point cur_pt = {-1,-1};//实时坐标
    char temp[16];
    if (event == CV_EVENT_LBUTTONDOWN)//左键按下，读取初始坐标，并在图像上该点处划圆
    {
        //将原始图片复制到img中
        sprintf(temp,"(%d,%d)",x,y);
        pt = Point(x,y);
        putText(img,temp,pre_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255),1,8);//在窗口上显示坐标
        circle(img,pre_pt,2,Scalar(255,0,0,0),CV_FILLED,CV_AA,0);//划圆
        imshow("src",img);
    }
    else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))//左键没有按下的情况下鼠标移动的处理函数
    {
        img.copyTo(tmp);//将img复制到临时图像tmp上，用于显示实时坐标
        sprintf(temp,"(%d,%d)",x,y);
        cur_pt = Point(x,y);
        putText(tmp,temp,cur_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));//只是实时显示鼠标移动的坐标
        imshow("src",tmp);
    }
}
string PDCounter::trainReadMe(){
    string s("提供两个函数\n第一个支持切割想要的图像，并且设置为正负样本，进行分类器训练\n第二个支持已有的正负样本文件夹，进行分类器训练\n此模块使得正常用户对于特定场景的训练成为可能\n");
    return s;
}// Return the help information of trainning


//void PDCounter::trainBySelf(String pic_file){
//    int posnum=0,negnum=0;char saveName[256];
//    _finddata_t file;
//    long lf;
//    char ss[256];
//    string suffix="\\*.*";
//    string xx=pic_file + suffix;
//    strcpy(ss,xx.c_str());
//    if((lf = _findfirst(ss, &file))==-1) cout<<"Not Found!"<<endl;
//    _findnext( lf, &file);
//    while(_findnext( lf, &file)==0){
//        String ImgName = pic_file+"\\"+file.name;
//        img = imread(ImgName);
//        Mat image_copy;
//        img.copyTo(image_copy);
//        imshow( "src", img );
//        int x,y;
//        int min_x,min_y,max_x,max_y;
//        int key;
//        double range=1;key = cvWaitKey(-1);
//        while(key!=27){
//            
//            if(key==32) break;
//            else if(key == 13){
//                key=cvWaitKey(-1);
//                if(key=='p'||key=='P'){
//                    Mat roi_of_img( img, Rect(min_y, min_x, max_y-min_y,max_x-min_x));
//                    resize(roi_of_img,roi_of_img,Size(96,160));
//                    sprintf(saveName,"pos_file\\person%.06d.jpg",++posnum);//生成裁剪出的负样本图片的文件名
//                    imwrite(saveName, roi_of_img);//保存文件
//                }
//                if(key=='n'||key=='N'){
//                    Mat roi_of_img( img, Rect(min_y, min_x, max_y-min_y,max_x-min_x));
//                    resize(roi_of_img,roi_of_img,Size(96,160));
//                    sprintf(saveName,"neg_file\\nonperson%.06d.jpg",++negnum);//生成裁剪出的负样本图片的文件名
//                    imwrite(saveName, roi_of_img);//保存文件
//                }
//            }
//            else if(key == 2490368){
//                range+=0.2;
//                min_x=max(0,(int)(x-80*range));
//                max_x=min_x+(int)160*range;
//                if(max_x>img.rows){ cout<<"impossible"<<endl;break;}
//                min_y = max(0,(int)(y-48*range));
//                max_y = min_y + (int)96*range;
//                if(max_y>img.cols){ cout<<"impossible"<<endl;break;}
//                Rect r(min_y,min_x,max_y-min_y,max_x-min_x);
//                image_copy.copyTo(img);
//                rectangle(img, r.tl(), r.br(), Scalar(0,255,0), 3);
//                imshow( "src", img );
//            }//放大
//            else if(key == 2621440){
//                range-=0.2;
//                if(range==0) continue;
//                min_x=max(0,(int)(x-80*range));
//                max_x=min_x+(int)160*range;
//                if(max_x>img.rows){ cout<<"impossible"<<endl;break;}
//                min_y = max(0,(int)(y-48*range));
//                max_y = min_y + (int)96*range;
//                if(max_y>img.cols){ cout<<"impossible"<<endl;break;}
//                Rect r(min_y,min_x,max_y-min_y,max_x-min_x);
//                image_copy.copyTo(img);
//                rectangle(img, r.tl(), r.br(), Scalar(0,255,0), 3);
//                imshow( "src", img );
//            }//缩小
//            else if(key == 'f'||key=='F'){
//                image_copy.copyTo(img);
//                setMouseCallback("src",on_mouse,0);//调用回调函数
//                key=cvWaitKey(-1);
//                range=1;
//                x=pt.y;y=pt.x;
//                min_x=max(0,x-80);
//                max_x=min_x+160;
//                if(max_x>img.rows){ cout<<"impossible"<<endl;break;}
//                min_y = max(0,y-48);
//                max_y = min_y + 96;
//                if(max_y>img.cols){ cout<<"impossible"<<endl;break;}
//                Rect r(min_y,min_x,max_y-min_y,max_x-min_x);
//                rectangle(img, r.tl(), r.br(), Scalar(0,255,0), 3);
//                imshow( "src", img);
//            }key = cvWaitKey(-1);
//        }if(key==27){
//            break;
//        }
//    }
//    trainBySelf("pos_file","neg_file");
//}// Choose the picture path to divide positive


//void PDCounter::trainBySelf(String pos_file, String neg_file){
//    int PosSamNO=0,NegSamNO=0;
//    HOGDescriptor hog(Size(64,128),Size(16,16),Size(8,8),Size(8,8),9);//HOG检测器，用来计算HOG描述子的
//    int DescriptorDim;//HOG描述子的维数，由图片大小、检测窗口大小、块大小、细胞单元中直方图bin个数决定
//    MySVM svm;//SVM分类器
//    string ImgName;//图片名(绝对路径)
//    //怎么获取文件名，需要修改
//    string suffix="\\*.*";
//    char Pos_file[]="Pos_file.txt";
//    char Neg_file[]="Neg_file.txt";
//    string xx=pos_file + suffix;
//    string yy=neg_file + suffix;
//    char temp[50];
//    
//    _finddata_t file;
//    ofstream fout;
//    long lf;
//    fout.open(Pos_file);
//    strcpy(temp,xx.c_str());
//    if((lf = _findfirst(temp, &file))==-1)    //这个里面需要修改
//        cout<<"Not Found!"<<endl;
//    else{
//        //    cout<<"file name list:"<<endl;
//        while(_findnext( lf, &file)==0){
//            fout<<file.name<<endl;
//            PosSamNO++;
//        }
//    }
//    
//    fout.close();
//    strcpy(temp,yy.c_str());
//    fout.open(Neg_file);
//    if((lf = _findfirst(temp, &file))==-1)    //这个里面需要修改
//        cout<<"Not Found!"<<endl;
//    else{
//        //    cout<<"file name list:"<<endl;
//        while(_findnext( lf, &file)==0){
//            fout<<file.name<<endl;
//            NegSamNO++;//写入到文件
//        }
//    }
//    fout.close();
//    _findclose(lf);
//    
//    PosSamNO--;
//    NegSamNO--;
//    
//    ifstream finPos(Pos_file);//正样本图片的文件名列表
//    ifstream finNeg(Neg_file);//负样本图片的文件名列表
//    
//    
//    Mat sampleFeatureMat;//所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数
//    Mat sampleLabelMat;//训练样本的类别向量，行数等于所有样本的个数，列数等于1；1表示有人，-1表示无人
//    
//    //依次读取正样本图片，生成HOG描述子
//    getline(finPos,ImgName);
//    for(int num=0; num<PosSamNO && getline(finPos,ImgName); num++)
//    {
//        cout<<"处理："<<ImgName<<endl;
//        //ImgName = "D:\\DataSet\\PersonFromVOC2012\\" + ImgName;//加上正样本的路径名
//        ImgName = pos_file+"\\"+ImgName;//加上正样本的路径名
//        cout<<ImgName<<endl;
//        Mat src = imread(ImgName);//读取图片
//        src = src(Rect(16,16,64,128));//将96*160的INRIA正样本图片剪裁为64*128，即剪去上下左右各16个像素
//        //resize(src,src,Size(64,128));
//        vector<float> descriptors;//HOG描述子向量
//        hog.compute(src,descriptors,Size(8,8));//计算HOG描述子，检测窗口移动步长(8,8)
//        cout<<"描述子维数："<<descriptors.size()<<endl;
//        
//        //处理第一个样本时初始化特征向量矩阵和类别矩阵，因为只有知道了特征向量的维数才能初始化特征向量矩阵
//        if(num==0){
//            DescriptorDim = (int)descriptors.size();//HOG描述子的维数
//            //初始化所有训练样本的特征向量组成的矩阵，行数等于所有样本的个数，列数等于HOG描述子维数sampleFeatureMat
//            sampleFeatureMat = Mat::zeros(PosSamNO+NegSamNO, DescriptorDim, CV_32FC1);
//            //初始化训练样本的类别向量，行数等于所有样本的个数，列数等于1；1表示有人，0表示无人
//            sampleLabelMat = Mat::zeros(PosSamNO+NegSamNO, 1, CV_32FC1);
//        }
//        
//        //将计算好的HOG描述子复制到样本特征矩阵sampleFeatureMat
//        for(int i=0; i<DescriptorDim; i++)
//            sampleFeatureMat.at<float>(num,i) = descriptors[i];//第num个样本的特征向量中的第i个元素
//        sampleLabelMat.at<float>(num,0) = 1;//正样本类别为1，有人
//    }
//    getline(finNeg,ImgName);
//    //依次读取负样本图片，生成HOG描述子
//    for(int num=0; num<NegSamNO && getline(finNeg,ImgName); num++){
//        cout<<"处理："<<ImgName<<endl;  
//        ImgName = neg_file +"\\"+ ImgName;//加上负样本的路径名  
//        Mat src = imread(ImgName);//读取图片  
//        src = src(Rect(16,16,64,128));//将96*160的INRIA正样本图片剪裁为64*128，即剪去上下左右各16个像素
//        //resize(src,img,Size(64,128));
//        vector<float> descriptors;//HOG描述子向量  
//        hog.compute(src,descriptors,Size(8,8));//计算HOG描述子，检测窗口移动步长(8,8)  
//        //cout<<"描述子维数："<<descriptors.size()<<endl;  
//        
//        //将计算好的HOG描述子复制到样本特征矩阵sampleFeatureMat  
//        for(int i=0; i<DescriptorDim; i++)  
//            sampleFeatureMat.at<float>(num+PosSamNO,i) = descriptors[i];//第PosSamNO+num个样本的特征向量中的第i个元素  
//        sampleLabelMat.at<float>(num+PosSamNO,0) = -1;//负样本类别为-1，无人  
//    }
//    
//    //训练SVM分类器  
//    //迭代终止条件，当迭代满1000次或误差小于FLT_EPSILON时停止迭代  
//    CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, FLT_EPSILON);  
//    //SVM参数：SVM类型为C_SVC；线性核函数；松弛因子C=0.01  
//    CvSVMParams param(CvSVM::C_SVC, CvSVM::LINEAR, 0, 1, 0, 0.01, 0, 0, 0, criteria);  
//    cout<<"开始训练SVM分类器"<<endl;  
//    svm.train(sampleFeatureMat,sampleLabelMat, Mat(), Mat(), param);//训练分类器  
//    cout<<"训练完成"<<endl;
//    svm.save("SVM_HOG.xml");
//}
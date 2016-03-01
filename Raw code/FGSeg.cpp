//
//  ForegroundDivision.cpp
//  PedestrianDetetion
//
//  Created by XMH on 1/31/16.
//  Copyright © 2016 XMH. All rights reserved.
//

#include "FGSeg.hpp"

#define threshold_diff 20

BackgroundSubtractorMOG2 bgSubtractor(20,16,true);


FGSeg::FGSeg(){
    re.clear();
    frame=0;
}
FGSeg::FGSeg(Rect t){
    re.clear();
    frame=0;
    total=t;
}
FGSeg::~FGSeg(){
    
}

bool FGSeg::setROI(Rect ROI){
    re.clear();
    if((ROI&total)!=ROI) return false;
    re.push_back(ROI);
    return true;
}

bool FGSeg::setROI(vector<Rect>& ROI){
    re.clear();
    for(int i=0; i<ROI.size(); i++)
    {
        Rect r = ROI[i];
        if((r&total)!=r) return false;
        re.push_back(r);
    }
    return true;
}
vector<Rect> FGSeg::detect(Mat& input){
    Mat gray1,gray2,gray_diff;
    vector<Rect> temp;
    /*
     uchar *pData1;
     temp.clear();
     if(frame==0){
     cvtColor(input,last,CV_BGR2GRAY);
     frame++;
     return temp;
     }
     gray1=last;
     
     cvtColor(input,gray2,CV_BGR2GRAY);
     last=gray2;
     
     //	imshow("video_src",input);
     subtract(gray1,gray2,gray_diff);
     for(int i=0;i<gray_diff.rows;i++){
     pData1=gray_diff.ptr<uchar>(i);
     for(int j=0;j<gray_diff.cols;j++){
     if(abs(pData1[j])>=threshold_diff) {
     pData1[j]=255;
     }else {
     pData1[j]=0;
     }
     }
     }
     */
    Mat result = input;
    
    bgSubtractor(input,gray_diff,0.001);
    
    morphologyEx(gray_diff,gray_diff,MORPH_CLOSE,Mat(3,3,CV_8U),Point(-1,-1),1);//����
    morphologyEx(gray_diff,gray_diff,MORPH_OPEN,Mat(3,3,CV_8U),Point(-1,-1),1);//��ʴ
    morphologyEx(gray_diff,gray_diff,MORPH_CLOSE,Mat(3,3,CV_8U),Point(-1,-1),1);//����
    imshow("gray",gray_diff);
    
    uchar *pData;
    int min_x=result.rows,max_x=0,min_y=result.cols,max_y=0;
    for(int i=0;i<result.rows;i++){
        pData=gray_diff.ptr<uchar>(i);
        for(int j=0;j<result.cols;j++){
            if(pData[j]==255) {
                min_x=min(min_x,i);min_y=min(min_y,j);max_x=max(max_x,i);max_y=max(max_y,j);
                pData++;
                
            }
        }
    }
    
    cout<<min_x<<" "<<max_x<<" "<<min_y<<" "<<max_y<<endl;
    Rect r(min_y,min_x,max_y-min_y,max_x-min_x);
    if (re.empty()) temp.push_back(r);
    else for(int i=0; i<re.size(); i++)
    {
        Rect R = re[i];
        temp.push_back(R&r);
    }
    
    for(int i=0; i<temp.size(); i++)
    {
        Rect r=temp[i];
        rectangle(result, r.tl(), r.br(), Scalar(0,255,0), 3);
    }
    imshow("result",result);
    return temp;
    
}
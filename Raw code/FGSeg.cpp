#include  "FGSeg.hpp"
#include <time.h>

BackgroundSubtractorMOG2 bgSubtractor(20,16,true);

FGSeg::FGSeg(){
    re.clear();frame=0;
}
FGSeg::~FGSeg(){
    
}

bool FGSeg::setROI(Rect ROI){
    if(frame==0) {
        cout<< "请等待第一帧后设置" <<endl;
        return false;
    }
    re.clear();
    if((ROI&total)!=ROI) return false;
    re.push_back(ROI);
    return true;
}

bool FGSeg::setROI(vector<Rect>& ROI){
    if(frame==0){
        cout<< "请等待第一帧后设置" <<endl;
        return false;
    }
    re.clear();
    for(int i=0; i<ROI.size(); i++)
    {
        Rect r = ROI[i];
        if((r&total)!=r) return false;
        re.push_back(r);
    }
    return true;
}
Mat FGSeg::getGray(Mat& input){
    Mat gray;
    cvtColor(input,gray,CV_BGR2GRAY);
    return gray;
}
Mat FGSeg::getMask(Mat& input){
    Mat gray_diff;
    bgSubtractor(input,gray_diff,0.001);
    morphologyEx(gray_diff,gray_diff,MORPH_OPEN,Mat(3,3,CV_8U),Point(-1,-1),1);//腐蚀
    morphologyEx(gray_diff,gray_diff,MORPH_CLOSE,Mat(3,3,CV_8U),Point(-1,-1),1);//膨胀
    imshow("gray",gray_diff);
    return gray_diff;
}
vector<Rect> FGSeg::detect(Mat& input){
    if(frame==0){
        total=Rect(0,0,input.cols,input.rows);
    }
    frame++;
    
    vector<Rect> temp,t;
    Mat result = input;
    Mat gray_diff=getMask(input);
    
    uchar *pData;
    int min_x=result.rows,max_x=0,min_y=result.cols,max_y=0;
    for(int i=0;i<result.rows;i++){
        pData=gray_diff.ptr<uchar>(i);
        for(int j=0;j<result.cols;j++){
            if(pData[j]==255) {
                min_x=min(min_x,i);min_y=min(min_y,j);max_x=max(max_x,i);max_y=max(max_y,j);
            }
        }
    }
    Rect r(min_y,min_x,max_y-min_y,max_x-min_x);
    t.push_back(r);
    if (re.empty()){
        for(int i=0;i<t.size();i++){
            Rect r=t[i];
            temp.push_back(r);
        }
    }
    else for(int i=0; i<re.size(); i++)
    {
        for(int j=0;j<t.size();j++){
            Rect r=t[j];
            Rect R = re[i];
            temp.push_back(R&r);
        }
    }
    
    //下面画出矩形框
    for(int i=0; i<temp.size(); i++)
    {
        Rect r=temp[i];
        rectangle(result, r.tl(), r.br(), Scalar(0,255,0), 3);
    }
    imshow("result",result);
    return temp;
}




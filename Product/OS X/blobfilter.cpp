#include "BlobFilter.h"
#include<queue>
using namespace std;
BlobFilter::BlobFilter(int set){
    choice=set;
}
BlobFilter::~BlobFilter(){
}
void BlobFilter::adjustAccuracy(unsigned int level){
}

const string BlobFilter::setFilter(int set){
    choice=set;
    string s;
    if(set==0) return "segment filter is set";
    else if(set==1) return "color filter is set";
    else if(set==2) return "shape filter is set";
    else return "set failed";
}

struct Node{
    int i,j;
};
int dr[]={-1,0,1,0};
int dc[]={0,1,0,-1};

vector<Rect> BlobFilter::filtrate(Mat& mask){
    vector<Rect> temp,t;
    temp.clear();
    Mat vis=Mat::zeros(mask.size(), CV_32FC1);
    uchar *vv;
    for(int i=0;i<mask.rows;i++){
        vv=vis.ptr<uchar>(i);
        for(int j=0;j<mask.cols;j++){
            vv[j]=0;
        }
    }
    uchar *pData;
    for(int i=0;i<mask.rows;i++){
        pData=mask.ptr<uchar>(i);
        vv=vis.ptr<uchar>(i);
        for(int j=0;j<mask.cols;j++){
            if((pData[j]==255)&&(vv[j]==0)) {
                int min_x=i,max_x=i,min_y=j,max_y=j;
                vv[j]=1;
                queue<Node> q;
                Node u={i,j};
                q.push(u);
                while(!q.empty()){
                    Node u=q.front();q.pop();
                    for(int k=0;k<4;k++){
                        Node v={u.i+dr[k],u.j+dc[k]};

                        if(v.i>=0&&v.i<mask.rows&&v.j>=0&&v.j<=mask.cols){
                            vv=vis.ptr<uchar>(v.i);pData=mask.ptr<uchar>(v.i);
                            if((vv[v.j]==0)&&(pData[v.j])){
                                vv[v.j]=1;
                                min_x=min(min_x,v.i);min_y=min(min_y,v.j);max_x=max(max_x,v.i);max_y=max(max_y,v.j);
                                q.push(v);
                            }
                        }
                    }
                }
    //			cout<<min_x<<" "<<max_x<<" "<<min_y<<" "<<max_y<<endl;
                Rect r(min_y,min_x,max_y-min_y,max_x-min_x);
                t.push_back(r);
            }
        }
    }

//show the rects
    for(int i=0;i<t.size();i++){
        Rect r=t[i];
        if(r.height*r.width>800) temp.push_back(r);
    }
    return temp;
}


//
//int main(int argc,unsigned char* argv[]){
//	VideoCapture cap("D:\\BaiduYunDownload\\测试视频\\4p-c0.avi");
//	if (!cap.isOpened()) return -1;
//
//	Mat img;
//	std::vector<Rect> mObjects;
//	FGSeg fd;
//	BlobFilter de;
//	namedWindow("video capture", CV_WINDOW_AUTOSIZE);
//
//	while (waitKey(20) < 0) { // Press any key to exit
//	/* code */
//		double start=clock();
//		cap >> img;
//		if (!img.data) continue;
//		imshow("video capture", img);
//		Mat x= fd.getMask(img);
//		mObjects= de.filtrate(x);
//		for(int i=0; i<mObjects.size(); i++){
//			Rect r=mObjects[i];
//			rectangle(img, r.tl(), r.br(), Scalar(0,255,0), 3);
//		}
//		imshow("result",img);
//
//		double end=clock();
//		printf("%lf\n",(end-start)/CLOCKS_PER_SEC);
//	}
//
//}


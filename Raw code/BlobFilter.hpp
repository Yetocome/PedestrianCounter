#ifndef BlobFilter_hpp
#define BlobFilter_hpp

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;

class BlobFilter {
public:
	BlobFilter(int set = 0);
	~BlobFilter();
	void adjustAccuracy(unsigned int level);
	const string setFilter(int set = 0);
	vector<Rect> filtrate(Mat& mask);
private:
	int choice;
};

#endif
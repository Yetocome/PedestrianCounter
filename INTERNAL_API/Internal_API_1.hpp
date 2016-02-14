#ifndef _____ForegroundDivision__
#define _____ForegroundDivision__

class ForegroundDivision {
public:
    ForegroundDivision();
    ~ForegroundDivision();

    bool setROI(Rect ROI);            // Set the Region of Interest in a picture
    bool setROI(vector<Rect>& ROI);   // Set the Regions of Interest in a picture
    vector<Rect> detect(Mat& input);  // The input mat should be sequential
                                      // Return the region of moving object in these picture
private:
	vector<Rect> re;
	int frame;
	Mat last;
};

#endif

//
// /* @Usage Module */
// VideoCapture cap(CV_CAP_ANY);
// if (!cap.isOpened())
//     return -1;
//
// Mat img;
// std::vector<Rect> mObjects;
// ForegroundDivision fd;
// fd.setROI(ANY_ROI); // Input the
// namedWindow("video capture", CV_WINDOW_AUTOSIZE);
//
// while (waitKey(20) < 0) { // Press any key to exit
//   /* code */
//   cap >> img;
//   if (!img.data)
//     continue;
//   mObjects = fd.detect(img);
//
//   imshow("video capture", show_img);
// }

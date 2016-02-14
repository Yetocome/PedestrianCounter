#ifndef _____PeopleCounter__
#define _____PeopleCounter__

class PeopleCounter {
public:
  PeopleCounter(int detector = 0);
  ~PeopleCounter();

  void adjust(int n); // Adjust the pareameter to accommodate user's environment
  string setDetector(int detector = 0); // Caution! Return value turns to the
                                        // name of detector

  // Caution! New function added
  string trainReadMe(); // Return the help information of trainning
  // The trainning functins are optional only if the Detector is default(or set 0)
  // Otherwise, make the option gray
  // If time limits, you can choose not to add this function
  // The graphic trainning procedure is not necessary, I will try some ways to implement
  // this in the function, so you don't have to write too many extra codes
  bool trainBySelf(String& pic_file); // Choose the picture path to divide positive
                                      // samples and negative samples
                                      // Fails if the pictures don't suit
  bool trainBySelf(String& pos_file, String& neg_file); // Choose exiting samples'
                                                        // path


  Mat& detect(Mat& input); // The input mat should be sequential

  int getCurrNum(); // Return the number of pedestrians on the screen
  int getObjNum(); // Return the number of objects which is moving on the screen
  int getPassedNum(); // Return the number of passed pedestrians from start

  void stop(); // Stop recording: this function will clear cache in the class
  void printLog(); // Print the current situation

private:

}
class Pedestrian {

}

#endif
//
// ////////////////////////////////////////////////////////////////////////////////
//
// /* @Usage Module */
// VideoCapture cap(CV_CAP_ANY);
// if (!cap.isOpened())
//     return -1;
//
// Mat img, show_img;
// int curr_num, passed_num;
// PeopleCounter pc;
// pc.setDetector(); // No input means using default detector
// namedWindow("video capture", CV_WINDOW_AUTOSIZE);
//
// while (waitKey(20) < 0) { // Press any key to exit
//   /* code */
//   cap >> img;
//   if (!img.data)
//     continue;
//   show_img = pc.detect(img);
//   curr_num = pc.getCurrNum();
//   passed_num = pc.getPassedNum();
//
//   imshow("video capture", show_img);
// }

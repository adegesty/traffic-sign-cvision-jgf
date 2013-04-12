#ifndef __THRESHOLDING_INCLUDED  
#define __THRESHOLDING_INCLUDED 

#include <opencv2/opencv.hpp>

using namespace cv;

void get_pure_color(const Mat &src, Mat &dst);
void shapeSmoothing(const Mat& src, Mat& dest);

#endif

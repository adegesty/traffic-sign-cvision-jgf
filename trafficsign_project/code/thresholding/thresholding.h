#ifndef __THRESHOLDING_INCLUDED  
#define __THRESHOLDING_INCLUDED 

#include <opencv2/opencv.hpp>

using namespace cv;

enum color
{
	BLUE = 1,
	GREEN = 2,
	RED = 4
};

void get_pure_color(const Mat &src, Mat &dst, int color);
void shapeSmoothing(const Mat& src, Mat& dest);

#endif

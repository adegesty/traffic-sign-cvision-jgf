/*
 * moments.cpp
 *
 *  Created on: Apr 12 2013
 *      Author: gautegam
 */

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
	Mat image;

	//Read image
	if(argc < 2){
		image = imread("../../example_images/shapes.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	}
	else{
		image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	}

	//Check for valid input
	if(!image.data){ 
		cout <<  "Could not open or find the image" << endl;
		return -1;
	}

	threshold(image, image, 128, 255, THRESH_BINARY);

	imshow("image", image);

	waitKey(0);
	return 0;
}

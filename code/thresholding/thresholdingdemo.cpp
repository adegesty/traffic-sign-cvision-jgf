/*
 * thresholding.cpp
 *
 *  Created on: Mar 4, 2013
 *      Author: fredrik
 */

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <iostream>
#include "thresholding.h"

using namespace cv;
using namespace std;


int main(int argc, char** argv){



	Mat image;
	//Read image
	if(argc < 2){
		image = imread("../../example_images/skilt_1.jpg", CV_LOAD_IMAGE_COLOR);
	}
	else{
		image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	}

	//Check for valid input
	if(!image.data){ 
		cout <<  "Could not open or find the image" << endl;
		return -1;
	}
	
	get_pure_color(image, image, RED);

	imshow("Thresholing demo", image);

	
	waitKey(0);

	return 0;
}


/*
 * thresholding.cpp
 *
 *  Created on: 5 april, 2013
 *      Author: jon åge
 */

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv){

	Mat image;

	//Read image
	if(argc < 2){
		image = imread("../../example_images/thresholded.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	}
	else{
		image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	}

	//Check for valid input
	if(!image.data){ 
		cout <<  "Could not open or find the image" << endl;
		return -1;
	}
	imshow("original image",image);
	Mat dilation_image;	
	Mat closing_image;
	Mat temp1;
	Mat finished_image;

	Mat element = getStructuringElement(MORPH_ELLIPSE,Size(6,6));

	//close
	dilate(image,dilation_image,element,Point(-1,-1),3);
	erode(dilation_image,closing_image,element,Point(-1,-1),3);
	//open
	erode(closing_image,temp1,element,Point(-1,-1),3);
	dilate(temp1,finished_image,element,Point(-1,-1),3);

	imshow("closed image",closing_image);
	imshow("test av erosion",finished_image);
	
	waitKey(0);	

	return 0;
}

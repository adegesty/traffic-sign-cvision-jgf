/*
 * thresholding.cpp
 *
 *  Created on: Mar 4, 2013
 *      Author: fredrik
 */

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <iostream>
#include <thresholding.h>

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
	
	//Get the red channel of the image
	Mat red(image.rows, image.cols, CV_8UC1);
	int fromTo[] = {2,0};
	mixChannels(&image, 1, &red, 1, fromTo, 1);

	//Get the grayscale of the image
	Mat grey(image.rows, image.cols, CV_8UC1);
	cvtColor(image, grey, CV_BGR2GRAY);

	//Remove other colors than real red
	Mat pureRed(image.rows, image.cols, CV_8UC1);
	pureRed = red - grey;

	//Do the thresholding
	Mat thresholded(image.rows, image.cols, CV_8UC1);

	//adaptiveThreshold(pureRed, thresholded, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, (pureRed.cols % 2 ? pureRed.cols : pureRed.cols - 1), -25);
	double thresholdValue;	
	//thresholdValue = mean(pureRed)[0]*4;
	minMaxLoc(pureRed, NULL, &thresholdValue);
	thresholdValue *= 0.65;

	threshold(pureRed, thresholded, thresholdValue, 255, THRESH_BINARY);

	imshow("Original image", image);
	imshow("Pure red image", pureRed);
	imshow("Thresholded", thresholded);

	waitKey(0);

	return 0;
}

void shapeSmoothing(const Mat& src, Mat& dest){

	Mat dilation_image;	
	Mat closing_image;
	Mat temp1;
	Mat finished_image;

	Mat element = getStructuringElement(MORPH_ELLIPSE,Size(6,6));

	//close
	dilate(src,dilation_image,element,Point(-1,-1),3);
	erode(dilation_image,closing_image,element,Point(-1,-1),3);
	//open
	erode(closing_image,temp1,element,Point(-1,-1),3);
	dilate(temp1,finished_image,element,Point(-1,-1),3);

	dest = temp1.clone();
}

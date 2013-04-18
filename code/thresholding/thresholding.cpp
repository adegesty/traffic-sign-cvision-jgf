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


void get_pure_color(const Mat &src, Mat &dst, int color){

	//Get the red channel of the src
	Mat color_channel(src.rows, src.cols, CV_8UC1);
	int fromTo[] = {0,0};
	switch(color){
		case BLUE:
			fromTo[0] = 0;
			mixChannels(&src, 1, &color_channel, 1, fromTo, 1);
			break;	
		case GREEN:
			fromTo[0] = 1;
			mixChannels(&src, 1, &color_channel, 1, fromTo, 1);
			break;	
		case RED:
			fromTo[0] = 2;
			mixChannels(&src, 1, &color_channel, 1, fromTo, 1);
			break;
	}
	//Get the grayscale of the src
	Mat grey(src.rows, src.cols, CV_8UC1);
	cvtColor(src, grey, CV_BGR2GRAY);

	//Remove other colors than real red
	Mat pureColor(src.rows, src.cols, CV_8UC1);
	pureColor = color_channel - grey;

	//Do the thresholding
	Mat thresholded(src.rows, src.cols, CV_8UC1);

	//adaptiveThreshold(pureRed, thresholded, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, (pureRed.cols % 2 ? pureRed.cols : pureRed.cols - 1), -25);
	double thresholdValue;	
	//thresholdValue = mean(pureRed)[0]*4;
	minMaxLoc(pureColor, NULL, &thresholdValue);
	thresholdValue *= 0.65;

	threshold(pureColor, thresholded, thresholdValue, 255, THRESH_BINARY);

	Mat smoothImage(src.rows, src.cols, CV_8UC1);

	//shapeSmoothing(thresholded,smoothImage);
//imwrite("../../example_images/images_circles/thresholded3.jpg",thresholded);

	dst =  thresholded.clone();
}


void shapeSmoothing(const Mat& src, Mat& dest){

	Mat dilation_image;	
	Mat closing_image;
	Mat temp1;
	Mat finished_image;

	Mat element = getStructuringElement(MORPH_ELLIPSE,Size(6,6));

	//close
	dilate(src,dilation_image,element,Point(-1,-1),1);
	erode(dilation_image,closing_image,element,Point(-1,-1),1);
	//open
	erode(closing_image,temp1,element,Point(-1,-1),1);
	dilate(temp1,finished_image,element,Point(-1,-1),1);

	dest = finished_image.clone();
}

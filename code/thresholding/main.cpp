/*
 * main.cpp
 *
 *  Created on: Mar 4, 2013
 *      Author: fredrik
 */

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv){

/*	if( argc != 2){
		cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
		return -1;
	}
*/



	Mat image;
	image = imread("skilt.jpg", CV_LOAD_IMAGE_COLOR);   // Read the file

	if(!image.data){                              // Check for invalid input
		cout <<  "Could not open or find the image" << std::endl;
		return -1;
	}

	Mat red(image.rows, image.cols, CV_8UC1);

	int fromTo[] = {2,0};

	mixChannels(&image, 1, &red, 1, fromTo, 1);

	Mat grey;
	cvtColor(image, grey, CV_BGR2GRAY);

	namedWindow("Original image", CV_WINDOW_AUTOSIZE);
	imshow("Original image", image);

	//namedWindow("Red image", CV_WINDOW_AUTOSIZE);
	//imshow("Red image", red);

	//namedWindow("Grey image", CV_WINDOW_AUTOSIZE);
	//imshow("Grey image", grey);

	Mat pureRed = red - grey;

	//namedWindow("Pure red image", CV_WINDOW_AUTOSIZE);
	//imshow("Pure red image", pureRed);

	Mat thresholded;

	//adaptiveThreshold(pureRed, thresholded, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, (pureRed.cols % 2 ? pureRed.cols : pureRed.cols - 1), -25);
	//threshold(pureRed, thresholded, 50, 255, THRESH_BINARY);
	double thresholdValue = mean(pureRed)[0]*4;
	threshold(pureRed, thresholded, thresholdValue, 1, THRESH_BINARY);

	Mat masked(image.rows, image.cols, CV_8UC3);


	namedWindow("Masked", CV_WINDOW_AUTOSIZE);
	imshow("Masked", masked);



	waitKey(0);

	return 0;
}

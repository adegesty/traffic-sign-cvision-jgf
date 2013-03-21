/*
 * colorSegmentation.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jon
 */

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <iostream>

using namespace cv;
using namespace std;


int main(int argc, char** argv){

	Mat image;
	image = imread("../../example_images/skilt_1.jpg", CV_LOAD_IMAGE_COLOR);

	if(!image.data){
		cout << "Could not open or find the image" << endl;
		return -1;
	}

	imshow("Original image",image);

	int width, height;
	height = image.rows;
	width = image.cols;

	Mat red(height,width,CV_8UC1);
	Mat blue(height,width,CV_8UC1);
	Mat green(height,width,CV_8UC1);
	vector<Mat> planes;
	split(image,planes);
	blue = planes[0];
	green = planes[1];
	red = planes[2];

	image.assignTo(image, CV_32FC3);
	
	float temp[3];
	float squareSum = 0;
	float threshold = 50*50*3;
	float maks = 0;

	//Normalize
	Mat normalized(image.rows, image.cols, CV_32FC3);

	for(int i = 0; i<image.rows;i++){
		for(int j = 0; j < image.cols;j++){
			for(int k = 0; k < 3; k++){			
				temp[k] = image.at<Vec3f>(i,j)[k];
				squareSum += temp[k]*temp[k];
				if (temp[k] > maks){
					maks = temp[k];
				}
			}
			for(int k = 0; k < 3; k++){
				if(squareSum > threshold){
					normalized.at<Vec3f>(i,j)[k] = temp[k]/maks;
				}else{
					normalized.at<Vec3f>(i,j)[k] = 0;
				}
			}
			squareSum = 0;
			maks = 0;
		}
	}

	imshow("Normalized image", normalized);

	waitKey(0);
	return 0;
}


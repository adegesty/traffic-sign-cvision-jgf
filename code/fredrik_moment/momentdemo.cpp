/*
 * thresholding.cpp
 *
 *  Created on: Apr 5 2013
 *      Author: fredrik
 */

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <iostream>
#include <vector>
#include "../region/region.h"
#include "moment.h"

using namespace cv;
using namespace std;


int main(int argc, char** argv){

	Mat image;

	//Read image
	if(argc < 2){
		image = imread("../../example_images/shapes_2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
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
	vector<Region> regions = find_regions(image);
	cout << "number of regions: " << regions.size() << endl;
	for(unsigned int i = 0; i < regions.size(); i++){
		double eig_val_1 = 0;
		double eig_val_2 = 0;
		get_covariance_eig_val(image(Range(regions[i].y_upper, regions[i].y_lower), Range(regions[i].x_leftmost, regions[i].x_rightmost)), &eig_val_1, &eig_val_2);
		Mat current_shape = image.clone();
		circle(current_shape, Point(regions[i].x_leftmost + ((regions[i].x_rightmost - regions[i].x_leftmost)/2), regions[i].y_upper + ((regions[i].y_lower - regions[i].y_upper)/2)), 5, Scalar(0,0,255), -1);
		cout << eig_val_2 << "," << eig_val_2 << endl;
		imshow("Momentdemo", current_shape);
		waitKey(0);
	}
	

	return 0;
}

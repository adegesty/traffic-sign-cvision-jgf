/*
 * momentdemo.cpp
 *
 * This is a program made to demonstrate the moment based
 *  shape recognition algorithm. It takes an image, filters the 
 *  wanted color and iterates through all regions, calculating the
 *  elipticity and triangularity of each.
 *  
 */

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <iostream>
#include <vector>
#include "../thresholding/thresholding.h"
#include "../region/region.h"
#include "moment.h"

using namespace cv;
using namespace std;


int main(int argc, char** argv){

	Mat image;

	//Read image
	if(argc < 2){
		image = imread("../../example_images/uk_50_mph_small.jpg", CV_LOAD_IMAGE_COLOR);
	}
	else{
		image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	}

	//Check for valid input
	if(!image.data){ 
		cout <<  "Could not open or find the image" << endl;
		return -1;
	}

	//Filter out the red. This function is from the thresholding module
	get_pure_color(image, image, RED);

	//Locate all regions. This function is from the region module
	vector<Region> regions = find_regions(image);
	cout << "number of regions: " << regions.size() << endl;

	for(unsigned int i = 0; i < regions.size(); i++){
		
		//Single out the current region
		Mat current_region = image(Range(regions[i].y_upper, regions[i].y_lower), Range(regions[i].x_leftmost, regions[i].x_rightmost));

		//Fill the region.
		fill_shape(current_region, current_region);

		//Mark the current region with a black dot in the middle
		Mat marked_image = image.clone();
		circle(marked_image, Point(regions[i].x_leftmost + ((regions[i].x_rightmost - regions[i].x_leftmost)/2), regions[i].y_upper + ((regions[i].y_lower - regions[i].y_upper)/2)), 5, Scalar(0), -1);

		//Compute invariance
		double invariance = get_invariance(current_region);

		//Show images and  print data
		imshow("current", current_region);
		cout << "Invariance: " << invariance << endl;
		cout << "Ellipticity: " << get_ellipticity(invariance) << endl;
		cout << "Triangularity: " << get_triangularity(invariance) << endl;
		imshow("Momentdemo", marked_image);
		waitKey(0);
	}
	

	return 0;
}

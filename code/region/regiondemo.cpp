/*
 * regiondemo.cpp
 *
 * This program demonstrates the region detection
 *  algorithm. It takes a greyscale image,
 *  finds each continuous region, and marks them with a
 *  square on the output picture. It also prints the
 *  total number of regions.
 *  
 */

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <iostream>
#include <vector>
#include "region.h"

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

	//Make the image binary
	threshold(image, image, 128, 255, THRESH_BINARY);

	//Find all regions
	vector<Region> regions = find_regions(image);
	cout << "number of regions: " << regions.size() << endl;
	
	//Mark each region
	for(unsigned int i = 0; i < regions.size(); i++){
		regions[i].print(image);		
	}

	//Show the image
	imshow("Region demo", image);
	imwrite("region_output.jpg", image);

	waitKey(0);

	return 0;
}

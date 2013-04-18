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

#include "../thresholding/thresholding.h"
#include "../region/region.h"

using namespace cv;
using namespace std;

void process_frame(const Mat &src, Mat &dst){
	get_pure_color(src, dst, RED);
	vector<Region> regions = find_regions(dst);
	for(unsigned int i = 0; i < regions.size(); i++){
		regions[i].print(dst);
	}
}


int main(int argc, char** argv){


	if(argc < 2){//Read default image file
		Mat image;
		image = imread("../../example_images/shapes.jpg", CV_LOAD_IMAGE_COLOR);
		//Check for valid input
		if(!image.data){ 
			cout <<  "Could not open or find the image" << endl;
			return -1;	
		}
		process_frame(image, image);
		imshow("Traffic sign detector", image);
		waitKey(0);
	}
	else if(strlen(argv[1]) == 1){//Start camera. Just give device number as argument
		
		VideoCapture cap(atoi(argv[1]));
        	if(!cap.isOpened()){ 
		        return -1;
		}
		namedWindow("Traffic sign detector",1);
		while(true){
			Mat frame;
			cap >> frame; 
			process_frame(frame, frame);
			imshow("Traffic sign detector", frame);
			if(waitKey(30) >= 0) break;
		}
	}
	else{//Read a given image file
		Mat image;
		image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
		//Check for valid input
		if(!image.data){ 
			cout <<  "Could not open or find the image" << endl;
			return -1;
		}
		process_frame(image, image);
		imshow("Traffic sign detector", image);
		waitKey(0);
	}

	return 0;
}

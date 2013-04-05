/*
 * thresholding.cpp
 *
 *  Created on: Apr 5 2013
 *      Author: fredrik
 */


/*

Finn en god måte å legge inn combine på.

*/



#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <iostream>
#include "region.h"

using namespace cv;
using namespace std;



int main(int argc, char** argv){



	Mat image;
	//Read image
	if(argc < 2){
		image = imread("../../example_images/shapes.jpg", CV_LOAD_IMAGE_COLOR);
	}
	else{
		image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	}

	//Check for valid input
	if(!image.data){ 
		cout <<  "Could not open or find the image" << endl;
		return -1;
	}

	int region_of_pixel[image.rows][image.cols];

	for(int i = 0; i < image.rows; i++){
		for(int j = 0; j < image.cols; j++){
			region_of_pixel[i][j] = 0;
		}
	}
	int next_region_number = 1;
	Vector<Region> regions;

	for(int i = 0; i < image.rows; i++){
		for(int j = 0; j < image.cols; j++){
			if(image.at<char>(i,j) != 0){
				bool check_done = false;
				for(int k = (i != 0 ? i-1 : 0); k <= i  && !check_done; k++){
					for(int l = (j != 0 ? j-1: 0); l <= (j != image.cols -1 ? j+1 : j); l++){
						if(k==i && l==j){
							check_done = true;
							break;
						}
						if(image.at<char>(k,l) != 0){
							if(region_of_pixel[i][j] == 0){
								region_of_pixel[i][j] = region_of_pixel[k][l];
							}
							else{
								//combine regions
								regions[region_of_pixel[i][j]].combine_with(regions[region_of_pixel[k][j]]);
							}
						}
					}
				}
				if(region_of_pixel[i][j] == 0){
					region_of_pixel[i][j] = next_region_number++;
					Region new_region;
					new_region.y_upper = i;
					new_region.y_lower = i;
					new_region.x_leftmost = j;
					new_region.x_rightmost = j;
					regions.add(new_region);
				}
			}
		}
	}

	return 0;
}

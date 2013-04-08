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
#include <vector>
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

	//Array to hold which region each pixel belongs to 0 means no region
	Region* region_of_pixel[image.rows][image.cols];

	for(int i = 0; i < image.rows; i++){
		for(int j = 0; j < image.cols; j++){
			region_of_pixel[i][j] = NULL;
		}
	}
	int next_region_number = 0;
	vector<Region*> regions;

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
							if(region_of_pixel[i][j] == NULL){
								region_of_pixel[i][j] = region_of_pixel[k][l];
							}
							else{
								//combine regions
								int region_to_delete = region_of_pixel[k][l]->label;
								region_of_pixel[k][l] = region_of_pixel[i][j];
								for(unsigned int m = 0; m < regions.size(); m++){
									if(regions[m]->label == region_to_delete){
										regions.erase(regions.begin() + m);
									}
								}
							}
						}
					}
				}
				if(region_of_pixel[i][j] == NULL){
					region_of_pixel[i][j] = new Region(next_region_number++);
	
					region_of_pixel[i][j]->y_upper = i;
					region_of_pixel[i][j]->y_lower = i;
					region_of_pixel[i][j]->x_leftmost = j;
					region_of_pixel[i][j]->x_rightmost = j;
					regions.push_back(region_of_pixel[i][j]);
				}
			}
		}
	}
	
	cout << regions.size() << endl;

	return 0;
}

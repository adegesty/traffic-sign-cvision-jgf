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

	threshold(image, image, 128, 255, THRESH_BINARY);
/*
	Mat image(5,5, CV_8UC1, Scalar(0));
	image.at<char>(1,1) = 255;
	image.at<char>(1,2) = 255;
	image.at<char>(1,3) = 255;
	image.at<char>(2,3) = 255;
	image.at<char>(3,3) = 255;
	image.at<char>(3,2) = 255;
	image.at<char>(3,1) = 255;

imshow("hei",image);
waitKey(0);
*/

	//Array to hold which region each pixel belongs to
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
			//Check if there is anything in the pixel
			if(image.at<char>(i,j) != 0){
				//Check pixels around
				bool check_done = false;
				for(int k = (i != 0 ? i-1 : 0); k <= i  && !check_done; k++){
					for(int l = (j != 0 ? j-1: 0); l <= (j != image.cols -1 ? j+1 : j); l++){
						if(k==i && l==j){
							check_done = true;
							break;
						}
						//Find neighbouring pixels
						if(image.at<char>(k,l) != 0){
							//If current pixel is not in any region, add to the neighbour region
							if(region_of_pixel[i][j] == NULL){
								region_of_pixel[i][j] = region_of_pixel[k][l];
								//cout << "adding pixel " << i << "," << j << " to region " << region_of_pixel[k][l]->label << endl;
								region_of_pixel[k][l]->pixels.push_back(Pixel(j,i));
								//Update region boundaries
								if (i < region_of_pixel[i][j]->y_upper){
									region_of_pixel[i][j]->y_upper = i;
								}
								if (i > region_of_pixel[i][j]->y_lower){
									region_of_pixel[i][j]->y_lower = i;
								}
								if (j < region_of_pixel[i][j]->x_leftmost){
									region_of_pixel[i][j]->x_leftmost = j;
								}
								if (j > region_of_pixel[i][j]->x_rightmost){
									region_of_pixel[i][j]->x_rightmost = j;
								}
							}
							//If current pixel has already been added to a region other than the new neighbor region, combine them.
							else if(region_of_pixel[i][j] != region_of_pixel[k][l]) {
								//cout << "combining regions at " << i << "," << j << endl;
								int region_to_delete;
								for(unsigned int m = 0; m < regions.size(); m++){
									if(regions[m]->label == region_of_pixel[k][l]->label){
										region_to_delete = m;
									}
								}

								region_of_pixel[i][j]->pixels.insert(region_of_pixel[i][j]->pixels.end(), region_of_pixel[k][l]->pixels.begin(), region_of_pixel[k][l]->pixels.end());
								for(unsigned int m = 0; m < region_of_pixel[k][l]->pixels.size(); m++){
									Pixel current_pixel = region_of_pixel[k][l]->pixels[m];
									region_of_pixel[current_pixel.y][current_pixel.x] = region_of_pixel[i][j];
								
									if (current_pixel.y < region_of_pixel[i][j]->y_upper){
										region_of_pixel[i][j]->y_upper = current_pixel.y;
									}
									if (current_pixel.y > region_of_pixel[i][j]->y_lower){
										region_of_pixel[i][j]->y_lower = current_pixel.y;
									}
									if (current_pixel.x < region_of_pixel[i][j]->x_leftmost){
										region_of_pixel[i][j]->x_leftmost = current_pixel.x;
									}
									if (current_pixel.x > region_of_pixel[i][j]->x_rightmost){
										region_of_pixel[i][j]->x_rightmost = current_pixel.x;
									}
								}//TODO update region boundaries
								//int region_to_delete = region_of_pixel[k][l]->label;
								//cout << "deleting region " << region_of_pixel[k][l]->label << endl;
								//region_of_pixel[k][l] = region_of_pixel[i][j];
								regions.erase(regions.begin() + region_to_delete);
								
							}
						}
					}
				}
				//If no neighboring pixels were found create a new region
				if(region_of_pixel[i][j] == NULL){
					region_of_pixel[i][j] = new Region(next_region_number++);
					region_of_pixel[i][j]->pixels.push_back(Pixel(j,i));
					//cout << "making new region at " << i << "," << j << " with label " << region_of_pixel[i][j]->label <<  endl;	
					region_of_pixel[i][j]->y_upper = i;
					region_of_pixel[i][j]->y_lower = i;
					region_of_pixel[i][j]->x_leftmost = j;
					region_of_pixel[i][j]->x_rightmost = j;
					regions.push_back(region_of_pixel[i][j]);
				}
			}
		}
	}
	
	cout << "number of regions: " << regions.size() << endl;
	
	for(unsigned int i = 0; i < regions.size(); i++){
		line(image, Point(regions[i]->x_leftmost, regions[i]->y_upper), Point(regions[i]->x_rightmost, regions[i]->y_upper), Scalar(255,255,255), 1, 8, 0);
		line(image, Point(regions[i]->x_leftmost, regions[i]->y_lower), Point(regions[i]->x_rightmost, regions[i]->y_lower), Scalar(255,255,255), 1, 8, 0);
		line(image, Point(regions[i]->x_leftmost, regions[i]->y_upper), Point(regions[i]->x_leftmost, regions[i]->y_lower), Scalar(255,255,255), 1, 8, 0);
		line(image, Point(regions[i]->x_rightmost, regions[i]->y_upper), Point(regions[i]->x_rightmost, regions[i]->y_lower), Scalar(255,255,255), 1, 8, 0);
	}
	imshow("hei", image);
	waitKey(0);

	return 0;
}

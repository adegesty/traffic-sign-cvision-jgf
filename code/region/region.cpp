#include <opencv2/opencv.hpp>
#include "region.h"
using namespace cv;

Region::Region(int _label){
	label = _label;
}




void Region::print(Mat& dest){
	line(dest, Point(x_leftmost, y_upper), Point(x_rightmost, y_upper), Scalar(255,255,255), 1, 8, 0);
	line(dest, Point(x_leftmost, y_lower), Point(x_rightmost, y_lower), Scalar(255,255,255), 1, 8, 0);
	line(dest, Point(x_leftmost, y_upper), Point(x_leftmost, y_lower), Scalar(255,255,255), 1, 8, 0);
	line(dest, Point(x_rightmost, y_upper), Point(x_rightmost, y_lower), Scalar(255,255,255), 1, 8, 0);
}

vector<Region> find_regions(const Mat& image){
	//lookup table to hold which region each pixel belongs to
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
								int region_to_delete;
								for(unsigned int m = 0; m < regions.size(); m++){
									if(regions[m]->label == region_of_pixel[k][l]->label){
										region_to_delete = m;
									}
								}
								
								//Append neighbors pixel vector to this regions vector
								region_of_pixel[i][j]->pixels.insert(region_of_pixel[i][j]->pixels.end(), region_of_pixel[k][l]->pixels.begin(), region_of_pixel[k][l]->pixels.end());

								//update lookup table
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
								}
								
								regions.erase(regions.begin() + region_to_delete);	
							}
						}
					}
				}
				//If no neighboring pixels were found create a new region
				if(region_of_pixel[i][j] == NULL){
					region_of_pixel[i][j] = new Region(next_region_number++);
					region_of_pixel[i][j]->pixels.push_back(Pixel(j,i));
					region_of_pixel[i][j]->y_upper = i;
					region_of_pixel[i][j]->y_lower = i;
					region_of_pixel[i][j]->x_leftmost = j;
					region_of_pixel[i][j]->x_rightmost = j;
					regions.push_back(region_of_pixel[i][j]);
				}
			}
		}
	}
	
	vector<Region> output;

	for(unsigned int i = 0; i < regions.size(); i++){
	
		Region new_region = *(regions[i]);
		new_region.label = i;
		output.push_back(new_region);

	}
	
	return output;
}

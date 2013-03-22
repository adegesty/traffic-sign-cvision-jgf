/*
 * colorSegmentation.cpp
 *
 *  Created on: Mar 12, 2013
 *      Author: jon
 */

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <iostream>
#include <queue>

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
/*
	Mat red(height,width,CV_8UC1);
	Mat blue(height,width,CV_8UC1);
	Mat green(height,width,CV_8UC1);
	vector<Mat> planes;
	split(image,planes);
	blue = planes[0];
	green = planes[1];
	red = planes[2];
*/
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

	Mat segmented = normalized.clone();
	
	float col_sum = 0;
	float mask_sum_5 = 0;
	float mask_sum_3 = 0;

	queue<float> temp_3_col_sums;
	queue<float> temp_5_col_sums;
	vector<Mat> planes;
	split(segmented,planes);

	float mean_3 = 0;
	float mean_5 = 0;

	float v1 = 0;
	float v2 = 0;

	float tol = 0;
	
	while(true){

	for(int img_row = 2; img_row < segmented.rows - 2; img_row++){
		for(int img_col = 2; img_col < segmented.cols - 2; img_col++){
			for(int color = 0; color < 3; color++){
				//For 5x5
				v1 = 0;
				v2 = 0;
				int mask_col_start = 0;
				if(temp_5_col_sums.size() == 5){
					mask_sum_5 -= temp_5_col_sums.front();
					temp_5_col_sums.pop();
					mask_col_start = img_col+2;
				}
				else{
					mask_col_start = img_col-2;
				}

				for(int mask_col = mask_col_start; mask_col <= img_col+2; mask_col++){
					for(int mask_row = img_row-2; mask_row <= img_row+2; mask_row++){
						col_sum += normalized.at<Vec3f>(mask_row,mask_col)[color];
					}
					temp_5_col_sums.push(col_sum);
					mask_sum_5 += col_sum;
				}
				mean_5 = mask_sum_5/25;

				float sum_2 = 0;
				float sum_3 = 0;

				for(int mask_col = img_col-2; mask_col <= img_col+2; mask_col++){
					for(int mask_row = img_row-2; mask_row <= img_row+2; mask_row++){
						sum_2 += (normalized.at<Vec3f>(mask_row, mask_col)[color] - mean_5)*(normalized.at<Vec3f>(mask_row, mask_col)[color] - mean_5);
						sum_3 += normalized.at<Vec3f>(mask_row, mask_col)[color] - mean_5;
					}
				}

				v2 += (sum_2 - sum_3*sum_3/25)/25;


					//For 3x3
				if(temp_3_col_sums.size() == 3){
					mask_sum_3 -= temp_3_col_sums.front();
					temp_3_col_sums.pop();
					mask_col_start = img_col+1;
				}
				else{
					mask_col_start = img_col-1;
				}

				for(int mask_col = mask_col_start; mask_col <= img_col+1; mask_col++){
					for(int mask_row = img_row-1; mask_row <= img_row+1; mask_row++){
						col_sum += normalized.at<Vec3f>(mask_row,mask_col)[color];
					}
					temp_3_col_sums.push(col_sum);
					mask_sum_3 += col_sum;
				}
				mean_3 = mask_sum_3/9;

				sum_2 = 0;
				sum_3 = 0;

				for(int mask_col = img_col-1; mask_col <= img_col+1; mask_col++){
					for(int mask_row = img_row-1; mask_row <= img_row+1; mask_row++){
						sum_2 += (normalized.at<Vec3f>(mask_row, mask_col)[color] - mean_3)*(normalized.at<Vec3f>(mask_row, mask_col)[color] - mean_3);
						sum_3 += normalized.at<Vec3f>(mask_row, mask_col)[color] - mean_3;
					}
				}

				v1 += (sum_2 - sum_3*sum_3/9)/9;
			
				if(v2 <= v1 + tol){
					segmented.at<Vec3f>(img_row, img_col)[color] = mean_3;
				}
			}
		}
	}
	imshow("Segmented",segmented);

	waitKey(0);
	}


	return 0;
}


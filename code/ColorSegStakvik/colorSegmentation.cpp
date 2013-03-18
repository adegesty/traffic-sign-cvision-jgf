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

/// Function Headers
//void update_map( void );
int ind = 0;

Mat dst, map_x,map_y;

int main(int argc, char** argv){

	Mat image;
	image = imread("../../example_images/skilt_1.jpg", CV_LOAD_IMAGE_COLOR);

	if(!image.data){
		cout << "Could not open or find the image" << endl;
		return -1;
	}

	//imshow("Original image",image);

	int width, height;
	height = image.rows;
	width = image.cols;

	Mat red(height,width,CV_8UC1);
	Mat blue(height,width,CV_8UC1);
	Mat green(height,width,CV_8UC1);
	
	image.assignTo(image, CV_32FC3);

	//int fromTo[] = {2,0};

	imshow("real image",image);

	//mixChannels(&image,1, &red, 1, fromTo, 1);

	vector<Mat> planes;
	split(image,planes);

	blue = planes[0];
	green = planes[1];
	red = planes[2];

	float temp[3];
	float squareSum = 0;
	float threshold = 50*50*3;
	float maks = 0;

	Mat normalised(image.rows, image.cols, CV_32FC3);

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
					//cout << temp[k]/maks << endl;
					normalised.at<Vec3f>(i,j)[k] = temp[k]/maks;
				}else{
					normalised.at<Vec3f>(i,j)[k] = 0;
				}
				//cout << normalised.at<Vec3f>(i,j)[k] << endl;
			}
			squareSum = 0;
			maks = 0;
		}
	}



	cout << normalised.at<Vec3f>(100,100) << endl;
	imshow("normalised image", normalised);


	waitKey(0);
	return 0;
}
/*

	void update_map(void){
		ind = ind%4;

		for (int i = 0; i < image.rows; i++){
			for (int j = 0; j < image.cols; j++){
				switch(ind){
		           case 0:
		             if( i > image.cols*0.25 && i < image.cols*0.75 && j > image.rows*0.25 && j < image.rows*0.75 )
		               {
		                 map_x.at<float>(j,i) = 2*( i - image.cols*0.25 ) + 0.5 ;
		                 map_y.at<float>(j,i) = 2*( j - image.rows*0.25 ) + 0.5 ;
		                }
		             else
		               { map_x.at<float>(j,i) = 0 ;
		                 map_y.at<float>(j,i) = 0 ;
		               }
		                 break;
		           case 1:
		                 map_x.at<float>(j,i) = i ;
		                 map_y.at<float>(j,i) = image.rows - j ;
		                 break;
		           case 2:
		                 map_x.at<float>(j,i) = image.cols - i ;
		                 map_y.at<float>(j,i) = j ;
		                 break;
		           case 3:
		                 map_x.at<float>(j,i) = image.cols - i ;
		                 map_y.at<float>(j,i) = image.rows - j ;
		                 break;
				}
			}
		}
		ind++;
	}
*/



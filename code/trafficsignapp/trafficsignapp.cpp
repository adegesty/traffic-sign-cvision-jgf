/*
 * trafficsignapp.cpp
 *
 * This progam combines all the modules, and allows
 *  the user to process the stream of a webcam or a still image.
 * 
 */

#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <iostream>
#include <vector>

#include "../thresholding/thresholding.h"
#include "../region/region.h"
#include "../moment/moment.h"

using namespace cv;
using namespace std;

//This is the function that is run on every frame
void process_frame(const Mat &src, Mat &dst){
	
	//Extract only the red parts of the image
	get_pure_color(src, dst, RED);

	//Identify all regions
	vector<Region> regions = find_regions(dst);
	
	for(unsigned int i = 0; i < regions.size(); i++){
		
		//Draw each region
		regions[i].print(dst);
		Mat current_region = src(Range(regions[i].y_upper, regions[i].y_lower), Range(regions[i].x_leftmost, regions[i].x_rightmost));
		
		//Fill the shape. This works for most noncircular shapes also
		fill_shape(current_region, current_region);
		
		//Calculate invariance for the shape, and print the numbers describing the shape
		double invariance = get_invariance(current_region);
		cout << "Invariance: " << invariance << endl;
		cout << "Ellipticity: " << get_ellipticity(invariance) << endl;
		cout << "Triangularity: " << get_triangularity(invariance) << endl;
	}
}


int main(int argc, char** argv){


	if(argc < 2){//Read default image file
		cout << "Usage: " << endl;
		cout << "\t\"" << argv[0] << " camera_number\" to start a stream from that camera" << endl; 
		cout << "\t\"" << argv[0] << " file_name \" to process a single image" << endl; 
		return 1;
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

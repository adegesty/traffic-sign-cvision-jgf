#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

#include "histogram.h"


using namespace std;
using namespace cv;

int main( int argc, char** argv ){
	Mat src;
	if(argc < 2){
		src = imread("../../example_images/skilt_1.jpg", CV_LOAD_IMAGE_COLOR);
	}
	else{
		src = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	}

	//Check for valid input
	if(!src.data){ 
		cout <<  "Could not open or find the image" << endl;
		return -1;
	}



	int hist_size = 256;
	int hist_width = 512; 
	int hist_height = 400;
	int bin_width = cvRound( (double) hist_width/hist_size );


	
	Histogram hist(src, hist_size, hist_width, hist_height, RED|GREEN|BLUE, 7);

	Mat hist_img;
	hist.draw_histogram(hist_img);

	vector<Peak> peaks;

	hist.get_peaks(&peaks, RED, 5);

line(hist_img, Point(0,hist_height - peaks[1].peak_val),
		  			  Point(hist_width, hist_height - peaks[1].peak_val),
		  			  Scalar(0,0,255), 1,8,0);

		line(hist_img, Point(bin_width*peaks[1].upper_index -1,hist_height), Point(bin_width*peaks[1].upper_index -1, 0), Scalar(0,0,255));
		
		line(hist_img, Point(bin_width*peaks[1].lower_index -1,hist_height), Point(bin_width*peaks[1].lower_index -1, 0), Scalar(0,0,255));
	
	
	vector<Mat> bgr_planes;
	split(src, bgr_planes);
	
	Mat thresholded;

	threshold(bgr_planes[2], thresholded, peaks[1].lower_index, 0, THRESH_TOZERO);

	threshold(thresholded, thresholded, peaks[1].upper_index, 0, THRESH_TOZERO_INV);

	/// Display
	//namedWindow("Histogram", CV_WINDOW_AUTOSIZE );
	imshow("Histogram", hist_img );

	imshow("Source image", src );
	imshow("Threshold", thresholded);
	//imshow("Threshold histogram", threshold_hist);
	waitKey(0);

	return 0;

}

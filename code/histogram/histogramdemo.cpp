/*
 * histogramdemo.cpp
 * 
 * This is a short program used to demonstrate the use of the 
 *  Histogram class in order to find different objects in
 *  an image based on their red-value.
 *
 * The program has been written loosely based on the opencv-tutorial at:
 *  http://docs.opencv.org/doc/tutorials/imgproc/histograms/histogram_calculation/histogram_calculation.html
 */

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

#include "histogram.h"


using namespace std;
using namespace cv;

int main( int argc, char** argv )
{
	//Read source image
	Mat src;
	if(argc < 2)
	{
		src = imread("../../example_images/skilt_1.jpg", CV_LOAD_IMAGE_COLOR);
	}
	else
	{
		src = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	}

	//Check for valid input
	if(!src.data)
	{ 
		cout <<  "Could not open or find the image" << endl;
		return -1;
	}

	//The scale of the histogram
	int hist_size = 256;

	//Size of the histogram image
	int hist_width = 512; 
	int hist_height = 400;

	//Number of histogram peaks we wish to examine
	int n_peaks = 5;

	//Make histogram from the source image
	Histogram hist(src, hist_size, hist_width, hist_height, RED|GREEN|BLUE, 7);

	//Draw the histogram
	Mat hist_img;
	hist.draw_histogram(hist_img);

	//Extract the peaks
	vector<Peak> peaks;
	hist.get_peaks(&peaks, RED, n_peaks);

	//Split image
	vector<Mat> bgr_planes;
	split(src, bgr_planes);

	//Iterate through peaks and show a gray-scale image of them
	for(int i = 0; i < n_peaks; i++)
	{
		Mat hist_with_current_peak = hist_img.clone();
		peaks[i].draw_peak(hist_with_current_peak, hist_size);

		Mat thresholded;
		threshold(bgr_planes[2], thresholded, peaks[i].lower_index, 0, THRESH_TOZERO);
		threshold(thresholded, thresholded, peaks[i].upper_index, 0, THRESH_TOZERO_INV);
	
		//Show images
		imshow("Source image", src);
		imshow("Histogram", hist_with_current_peak);
		imshow("Threshold", thresholded);
		waitKey(0);
	}
	return 0;
}

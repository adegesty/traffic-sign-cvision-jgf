/**
 *
 *
 *
 */

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

enum colors{
	BLUE = 1,
	GREEN = 2,
	RED = 4
};


void createHistogram(const Mat& src, Mat& dst, int color, int hist_size = 256){
	//Split the image
	//TODO: Gjør dette med mixChannels
	vector<Mat> bgr_planes;
	split(src, bgr_planes);
	
	//Initialize the range array
	float range[] = {0, hist_size};
	const float* hist_range = {range};
	
	switch(color){
		case BLUE:
			color = 0;
			break;
		case GREEN:
			color = 1;
			break;
		case RED:
			color = 2;
			break;
	}

	Mat output;
	calcHist(&bgr_planes[color], 1, 0, Mat(), output, 1, &hist_size, &hist_range, true, false);
	dst = output.clone();	

}


void smoothHistogram(Mat hist, int filterSize){
	if(filterSize == 0){
		return;
	}
	float partSum = 0;

	for(int i = 0; i < hist.rows; i++){
		//Motherfucking grenseverdier
		int j_lower = (i >= filterSize/2 ? -(filterSize/2) : 0);
		int j_upper = (i <= hist.rows - filterSize/2 ? filterSize /2 : hist.rows-i);

		for(int j = j_lower; j <= j_upper; j++){
			partSum += hist.at<float>(i + j);
		}
		hist.at<float>(i) = partSum/filterSize;
		partSum = 0;
	}

}

void drawHistogram(const Mat& hist, Mat& dst, int hist_size, int bin_width, int color){
	switch(color){
		case BLUE:
			for(int i = 1; i < hist_size; i++){
				line(dst, Point(bin_width*(i-1), dst.rows - cvRound(hist.at<float>(i-1))),
			  		  Point(bin_width*i, dst.rows - cvRound(hist.at<float>(i))),
			  		  Scalar(255,0,0), 2,8,0);
			}
			break;
		case GREEN:
			for(int i = 1; i < hist_size; i++){
				line(dst, Point(bin_width*(i-1), dst.rows - cvRound(hist.at<float>(i-1))),
			  		  Point(bin_width*i, dst.rows - cvRound(hist.at<float>(i))),
			  		  Scalar(0,255,0), 2,8,0);
			}
			break;
		case RED:
			for(int i = 1; i < hist_size; i++){
				line(dst, Point(bin_width*(i-1), dst.rows - cvRound(hist.at<float>(i-1))),
			  		  Point(bin_width*i, dst.rows - cvRound(hist.at<float>(i))),
			  		  Scalar(0,0,255), 2,8,0);
			}
			break;
		
	}
}

void quickHistogram(const Mat& src, Mat& dst, int hist_size, int hist_width, int hist_height, int colors, 
		    int smoothing_filter_size){
	Mat output(hist_height, hist_width, CV_8UC3, Scalar(0,0,0));
	int bin_width = cvRound((double)hist_width/hist_size);
	if(colors & BLUE){
		Mat b_hist;
		createHistogram(src, b_hist, BLUE, hist_size);
		normalize(b_hist, b_hist, 0, output.rows, NORM_MINMAX, -1, Mat());
		smoothHistogram(b_hist, smoothing_filter_size);
		drawHistogram(b_hist, output, hist_size, bin_width, BLUE);
	}

	if(colors & GREEN){
		Mat g_hist;
		createHistogram(src, g_hist, GREEN, hist_size);
		normalize(g_hist, g_hist, 0, output.rows, NORM_MINMAX, -1, Mat());
		smoothHistogram(g_hist, smoothing_filter_size);
		drawHistogram(g_hist, output, hist_size, bin_width, GREEN);
	}
	
	if(colors & RED){
		Mat r_hist;
		createHistogram(src, r_hist, RED, hist_size);
		normalize(r_hist, r_hist, 0, output.rows, NORM_MINMAX, -1, Mat());
		smoothHistogram(r_hist, smoothing_filter_size);
		drawHistogram(r_hist, output, hist_size, bin_width, RED);
	}

	dst = output.clone();

}

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
	int hist_w = 512; 
	int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/hist_size );


	Mat histImage;
	quickHistogram(src, histImage, hist_size, hist_w, hist_h, RED|GREEN|BLUE, 7);

	Mat r_hist;
	createHistogram(src, r_hist, RED, hist_size);

	float currentHighestValue = 0;
	int currentHighestIndex = 0;
//	for(int i = 0; i < 5; i++){
		for (int j = 0; j < r_hist.rows; j++){
			if(r_hist.at<float>(j) > currentHighestValue){
				currentHighestValue = r_hist.at<float>(j);
				currentHighestIndex = j;
			}
		}
		int lowerPeakBound = currentHighestIndex;
		int upperPeakBound = currentHighestIndex;
		line(histImage, Point(bin_w*currentHighestIndex-1,hist_h), Point(bin_w*currentHighestIndex -1, 0), Scalar(0,0,255), 2);
		while(r_hist.at<float>(lowerPeakBound-1) <  r_hist.at<float>(lowerPeakBound)){
			lowerPeakBound--;
		}
		line(histImage, Point(bin_w*lowerPeakBound-1,hist_h), Point(bin_w*lowerPeakBound -1, 0), Scalar(0,0,255));
		while(r_hist.at<float>(upperPeakBound+1) <  r_hist.at<float>(upperPeakBound)){
			upperPeakBound++;
		}
		line(histImage, Point(bin_w*upperPeakBound -1,hist_h), Point(bin_w*upperPeakBound -1, 0), Scalar(0,0,255));

		Mat thresholded;

		threshold(r_hist, thresholded, lowerPeakBound, 0, THRESH_TOZERO);

		threshold(thresholded, thresholded, upperPeakBound, 0, THRESH_TOZERO_INV);

//	}

	Mat threshold_hist;
	quickHistogram(thresholded, threshold_hist, hist_size, hist_w, hist_h, RED|GREEN|BLUE, 0);

	/// Display
	namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
	imshow("calcHist Demo", histImage );

	imshow("Source image", src );
	imshow("Threshold", thresholded);
	imshow("Threshold histogram", threshold_hist);
	waitKey(0);

	return 0;

}

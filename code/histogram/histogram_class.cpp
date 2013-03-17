#include "histogram.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

Histogram::Histogram(const Mat& _src, int _hist_size, int _hist_width, int _hist_height, int colors, 
			  int _smoothing_filter_size)
{
	src = _src;
	hist_size = _hist_size;
	hist_width = _hist_width;
	hist_height = _hist_height;
	bin_width = cvRound((double) hist_width/hist_size);
	smoothing_filter_size = _smoothing_filter_size;			  
	if(colors & BLUE){
		blue = true;
	}

	if(colors & GREEN){
		green = true;
	}
	
	if(colors & RED){
		red = true;
	}
			  
}

void Histogram::draw_histogram(Mat& dst)
{

}

int Histogram::get_peak(int color, int& peak, int& lower_bound, int& upper_bound, int start_below)
{

}

void Histogram::generate_histogram()
{
	//Split the image
	//TODO: Gjør dette med mixChannels
	vector<Mat> bgr_planes;
	split(src, bgr_planes);
	
	//Initialize the range array
	float range[] = {0, hist_size};
	const float* hist_range = {range};
	
	if(blue){
		calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &hist_size, &hist_range, true, false);
	}
	if(green){
		calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &hist_size, &hist_range, true, false);
	}
	if(red){
		calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &hist_size, &hist_range, true, false);
	}

}

void Histogram::smooth_histogram(Mat& hist)
{
	if(smoothing_filter_size == 0){
		return;
	}
	float partSum = 0;

	for(int i = 0; i < hist.rows; i++){
		//Motherfucking grenseverdier
		int j_lower = (i >= smoothing_filter_size/2 ? -(smoothing_filter_size/2) : 0);
		int j_upper = (i <= hist.rows - smoothing_filter_size/2 ? smoothing_filter_size /2 : hist.rows-i);

		for(int j = j_lower; j <= j_upper; j++){
			partSum += hist.at<float>(i + j);
		}
		hist.at<float>(i) = partSum/smoothing_filter_size;
		partSum = 0;
	}
}

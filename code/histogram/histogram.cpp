#include "histogram.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void Peak::draw_peak(Mat& img, int hist_size)
{

	int bin_width = cvRound( (double) img.cols/hist_size );
	
	line(img, Point(0, img.rows - peak_val), Point(img.cols, img.rows - peak_val), Scalar(255,255,255), 1,8,0);

	line(img, Point(bin_width*upper_index -1,img.rows), Point(bin_width*upper_index -1, 0), Scalar(255,255,255), 1, 8, 0);
	
	line(img, Point(bin_width*lower_index -1,img.rows), Point(bin_width*lower_index -1, 0), Scalar(255,255,255), 1, 8, 0);



}
Histogram::Histogram(const Mat& _src, int _hist_size, int _hist_width, int _hist_height, int colors, 
			  int _smoothing_filter_size)
{
	src = _src;
	hist_size = _hist_size;
	hist_width = _hist_width;
	hist_height = _hist_height;
	bin_width = cvRound((double) hist_width/hist_size);
	smoothing_filter_size = _smoothing_filter_size;			  
	blue = colors & BLUE;
	green = colors & GREEN;
	red = colors & RED;
	generate_histogram();			  

}

void Histogram::draw_histogram(Mat& dst)
{
	Mat output (hist_height, hist_width, CV_8UC3, Scalar(0,0,0));
	if(blue)
	{
		for(int i = 1; i < hist_size; i++){
			line(output, Point(bin_width*(i-1), output.rows - cvRound(b_hist.at<float>(i-1))),
		  			  Point(bin_width*i, output.rows - cvRound(b_hist.at<float>(i))),
		  			  Scalar(255,0,0), 2,8,0);
		}
	}	
	if(green){
		for(int i = 1; i < hist_size; i++){
			line(output, Point(bin_width*(i-1), output.rows - cvRound(g_hist.at<float>(i-1))),
		  			  Point(bin_width*i, output.rows - cvRound(g_hist.at<float>(i))),
		  			  Scalar(0,255,0), 2,8,0);
		}
	}
	if(red)
	{
		for(int i = 1; i < hist_size; i++){
			line(output, Point(bin_width*(i-1), output.rows - cvRound(r_hist.at<float>(i-1))),
		  			  Point(bin_width*i, output.rows - cvRound(r_hist.at<float>(i))),
		  			  Scalar(0,0,255), 2,8,0);
		}
	}		
	dst = output.clone();
}

void Histogram::get_peaks(vector<Peak> *peaks, int color, unsigned int n_peaks = 5)
{
	Mat hist;
	switch(color){
		//These checks should be done with exeption-throwing, 
		//but since its only for out project its not so important
		case BLUE:
			if(!blue){
				return;
			}
			hist = b_hist;
			break;
		case GREEN:
			if(!green){
				return;
			}
			hist = g_hist;
			break;
		case RED:
			if(!red){
				return;
			}
			hist = r_hist;
			break;
		default:
			return;
	}

	enum {SEARCH_BOTTOM, SEARCH_TOP} search_state;
	Peak *current_peak;
	if(hist.at<float>(0) <= hist.at<float>(1)){
		current_peak = new Peak;
		current_peak->lower_index = 0;
		search_state = SEARCH_TOP;
	}
	else{
		current_peak = new Peak;
		current_peak->lower_index = 0;
		current_peak->peak_val = hist.at<float>(0);
		search_state = SEARCH_BOTTOM;
	}
	
	for (int i = 1; i<hist.rows-1; i++){
		switch(search_state){
			case SEARCH_BOTTOM:
				if(hist.at<float>(i-1) > hist.at<float>(i) && hist.at<float>(i+1) > hist.at<float>(i))
				{
					current_peak->upper_index = i;
					peaks->push_back(*current_peak);
					delete current_peak;
					current_peak = new Peak;
					current_peak->lower_index = i;
					search_state = SEARCH_TOP;
				}
				break;
			case SEARCH_TOP:	
				if(hist.at<float>(i-1) < hist.at<float>(i) && hist.at<float>(i+1) < hist.at<float>(i))
				{
					current_peak->peak_val = hist.at<float>(i);
					search_state = SEARCH_BOTTOM;
				}
		}
	}

	if(search_state == SEARCH_BOTTOM)
	{
		current_peak->upper_index = hist.rows;
		peaks->push_back(*current_peak);
	}
	else
	{
		current_peak->peak_val = hist.at<float>(hist.rows);
		current_peak->upper_index = hist.rows;
		peaks->push_back(*current_peak);
	}
	delete current_peak;	

	sort(peaks->begin(), peaks->end());
	
	while(peaks->size() > n_peaks){
		peaks->pop_back();
	}

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
		normalize(b_hist, b_hist, 0, hist_size, NORM_MINMAX, -1, Mat());
	}
	if(green){
		calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &hist_size, &hist_range, true, false);
		normalize(g_hist, g_hist, 0, hist_size, NORM_MINMAX, -1, Mat());
	}
	if(red){
		calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &hist_size, &hist_range, true, false);
		normalize(r_hist, r_hist, 0, hist_size, NORM_MINMAX, -1, Mat());
	}
	
	smooth_histogram();
}

void Histogram::smooth_histogram()
{
	if(smoothing_filter_size == 0){
		return;
	}
	float partSum = 0;
	if(blue)
	{
		for(int i = 0; i < b_hist.rows; i++){
			//Motherfucking grenseverdier
			int j_lower = (i >= smoothing_filter_size/2 ? -(smoothing_filter_size/2) : 0);
			int j_upper = (i <= b_hist.rows - smoothing_filter_size/2 ? smoothing_filter_size /2 : b_hist.rows-i);

			for(int j = j_lower; j <= j_upper; j++){
				partSum += b_hist.at<float>(i + j);
			}
			b_hist.at<float>(i) = partSum/smoothing_filter_size;
			partSum = 0;
		}
	}
	if(green)
	{
		for(int i = 0; i < g_hist.rows; i++){
			//Motherfucking grenseverdier
			int j_lower = (i >= smoothing_filter_size/2 ? -(smoothing_filter_size/2) : 0);
			int j_upper = (i <= g_hist.rows - smoothing_filter_size/2 ? smoothing_filter_size /2 : g_hist.rows-i);

			for(int j = j_lower; j <= j_upper; j++){
				partSum += g_hist.at<float>(i + j);
			}
			g_hist.at<float>(i) = partSum/smoothing_filter_size;
			partSum = 0;
		}
	}
	if(red)
	{
		for(int i = 0; i < r_hist.rows; i++){
			//Motherfucking grenseverdier
			int j_lower = (i >= smoothing_filter_size/2 ? -(smoothing_filter_size/2) : 0);
			int j_upper = (i <= r_hist.rows - smoothing_filter_size/2 ? smoothing_filter_size /2 : r_hist.rows-i);

			for(int j = j_lower; j <= j_upper; j++){
				partSum += r_hist.at<float>(i + j);
			}
			r_hist.at<float>(i) = partSum/smoothing_filter_size;
			partSum = 0;
		}
	}
}

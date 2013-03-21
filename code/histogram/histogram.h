#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

enum colors
{
	BLUE = 1,
	GREEN = 2,
	RED = 4
};

//A class describing a peak in a histogram
class Peak
{
	public:
		//Draw the peak on a histogram image
		void draw_peak(Mat& img, int hist_size);

		//The value of the top of the peak
		double peak_val;

		//The index where the peak starts and ends
		int lower_index;
		int upper_index;
		
		//Operator overloading needed for sorting
		bool operator < (const Peak& other) const
		{
			return peak_val > other.peak_val;
		}
};

//A class for a basic RGB-histogram
class Histogram
{
	public:
		Histogram(const Mat& _src, int _hist_size, int _hist_width, int _hist_height, int colors, 
			  int _smoothing_filter_size);
	
		void draw_histogram(Mat& dst);
		
		//Puts the n_peaks highest peaks of the color histogram in the peaks vector
		void get_peaks(std::vector<Peak> *peaks, int color, unsigned int n_peaks);


	private:
		//Source image
		Mat src;

		//Histograms for each channel
		Mat b_hist;
		Mat g_hist;
		Mat r_hist;
		
		//The scale of the histogram
		int hist_size;

		int hist_width;
		int hist_height;
		int bin_width;

		//If this i 0, there is no smoothing
		int smoothing_filter_size;

		//Weather or not the different channels are used
		bool blue;
		bool green;
		bool red;

		void generate_histogram();
		void smooth_histogram();
};

#endif

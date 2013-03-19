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

class Peak
{

	public:
		double peak_val;
		int lower_index;
		int upper_index;
		
		bool operator < (const Peak& other) const
		{
			return peak_val > other.peak_val;
		}

};

class Histogram
{

	public:
		Histogram(const Mat& _src, int _hist_size, int _hist_width, int _hist_height, int colors, 
			  int _smoothing_filter_size);
	
		void draw_histogram(Mat& dst);

		void get_peaks(std::vector<Peak> *peaks, int color, unsigned int n_peaks);


	private:
		Mat src;
		Mat b_hist;
		Mat g_hist;
		Mat r_hist;

		int hist_size;
		int hist_width;
		int hist_height;
		int bin_width;
		int smoothing_filter_size;
		bool blue;
		bool green;
		bool red;

		void generate_histogram();
		void smooth_histogram();
};



#endif

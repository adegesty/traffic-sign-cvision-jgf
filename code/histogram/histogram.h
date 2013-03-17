

#include <opencv2/opencv.hpp>

using namespace cv;
enum colors
{
	BLUE = 1,
	GREEN = 2,
	RED = 4
};

class Histogram
{

	public:
		Histogram(const Mat& _src, int _hist_size, int _hist_width, int _hist_height, int colors, 
			  int _smoothing_filter_size);
	
		void draw_histogram(Mat& dst);

		//Finds the highest peak below start_below, returns the peak value
		int get_peak(int color, int& peak, int& lower_bound, int& upper_bound, int start_below);


	private:
		Mat src;
		Mat b_hist;
		Mat g_hist;
		Mat r_hist;

		int hist_size;
		int hist_width;
		int hist_height;
		int bin_width;
		bool blue;
		bool green;
		bool red;

		void generate_histogram();
		void smooth_histogram();
};

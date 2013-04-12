#ifndef REGION_H
#define REGION_H

#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;
using namespace std;

class Pixel{

	public: 
		Pixel(int _x, int _y) {x = _x; y = _y;}
		int x;
		int y;

};

class Region {

	public:

		Region(int _label);
		void print(Mat& dest);	
		void combine_with(Region *other_region);

		int label;

		vector<Pixel> pixels;
		int y_upper;
		int y_lower;
		int x_leftmost;
		int x_rightmost;

};

vector<Region> find_regions(const Mat& image);

#endif

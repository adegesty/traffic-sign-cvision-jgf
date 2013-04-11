#ifndef REGION_H
#define REGION_H

#include <vector>
class Pixel{

	public: 
		Pixel(int _x, int _y) {x = _x; y = _y;}
		int x;
		int y;

};

class Region {

	public:

		Region(int _label);
		
		void combine_with(Region other_region);

		int label;

		std::vector<Pixel> pixels;
		int y_upper;
		int y_lower;
		int x_leftmost;
		int x_rightmost;

};


#endif

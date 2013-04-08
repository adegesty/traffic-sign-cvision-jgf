#ifndef REGION_H
#define REGION_H


class Region {

	public:

		Region(int _label);
		
		void combine_with(Region other_region);

		int label;

		int y_upper;
		int y_lower;
		int x_leftmost;
		int x_rightmost;

};


#endif

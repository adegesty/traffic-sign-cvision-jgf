#ifndef REGION_H
#define REGION_H


class Region {

	public:

		Region();
		
		void combine_with(Region other_region);


		int y_upper;
		int y_lower;
		int x_leftmost;
		int x_rightmost;

};


#endif

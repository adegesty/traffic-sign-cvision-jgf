/*
 * moments.cpp
 *
 *  Created on: Apr 12 2013
 *      Author: gautegam
 */

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	  Mat src, dst;
	  /// Load image
	  src = imread( argv[1], 1 );
	  if( !src.data ) { return -1; }
	  imshow( "Display Image", src );

	  //Moments:
	  double m_00 = 0;
	  double m_10 = 0;
	  double m_01 = 0;
	  //Central moments
	  double my_00 = 0;
	  double my_11 = 0;
	  double my_20 = 0;
	  double my_02 = 0;

	  for(int i=0; i<src.rows; i++) {
		  for(int j=0; j<src.cols; j++) {
			  m_00 += (int)src.at<Vec3b>(i,j)[0];
		  }
	  }
	  cout << m_00 << endl;

	  waitKey(0);
	  return 0;
}

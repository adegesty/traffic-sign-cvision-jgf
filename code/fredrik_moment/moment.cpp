#include <opencv2/opencv.hpp>
#include <cmath>

#include "moment.h"

using namespace cv;


int get_moment(const Mat& src, int p, int q){
	long int moment = 0;
	for(int i = 0; i < src.rows; i++){
		for(int j = 0; j < src.cols; j++){
			if(src.at<char>(i,j) != 0){
				moment += pow(i,p)*pow(j,q);
			}
		}
	}
	return moment;
}

double get_central_moment(const Mat& src, int p, int q){
	double moment = 0;
	
	int m_00 = get_moment(src, 0, 0);
	int m_10 = get_moment(src, 1, 0);
	int m_01 = get_moment(src, 0, 1);
	double center_of_mass_x = ((double)m_10)/m_00;
	double center_of_mass_y = ((double)m_01)/m_00;

	for(int i = 0; i < src.rows; i++){
		for(int j = 0; j < src.cols; j++){
			if(src.at<char>(i,j) != 0){
				moment += pow(i-center_of_mass_x,p)*pow(j-center_of_mass_y,q);
			}
		}
	}

	return moment;
}

void get_covariance_eig_val(const Mat& src, double *eig_val_1, double *eig_val_2){
	
	double mu_00 = get_central_moment(src, 0,0);
	double mu_11 = get_central_moment(src, 1,1);
	double mu_20 = get_central_moment(src, 2,0);
	double mu_02 = get_central_moment(src, 0,2);

	double mu_20_bar = mu_20/mu_00;
	double mu_02_bar = mu_02/mu_00;
	double mu_11_bar = mu_11/mu_00;

	double first_term_of_eig_func = (mu_20_bar+mu_02_bar)/2;
	double second_term_of_eig_func = sqrt(pow((mu_20_bar - mu_02_bar)/2,2)+pow(mu_11_bar,2));
	
	*eig_val_1 = (first_term_of_eig_func + second_term_of_eig_func);
	*eig_val_2 = (first_term_of_eig_func - second_term_of_eig_func);
}

double get_invariance(const Mat& src){
	
	double mu_00 = get_central_moment(src, 0,0);
	double mu_11 = get_central_moment(src, 1,1);
	double mu_20 = get_central_moment(src, 2,0);
	double mu_02 = get_central_moment(src, 0,2);

	return ((mu_20*mu_02-pow(mu_11,2))/pow(mu_00,4));

}


double get_ellipticity(double invariance){
	double perfect_ellipse_invariance = 0.00633257397;
	
	if(invariance <= perfect_ellipse_invariance){
		return (1/perfect_ellipse_invariance)*invariance;
	}

	return (perfect_ellipse_invariance)*(1/invariance);
}
/*double get_rectangularity(double invariance){

}*/

double get_triangularity(double invariance){
	
	double perfect_triangle_invariance = 1.0/108.0;
	
	if(invariance <= perfect_triangle_invariance){
		return (1/perfect_triangle_invariance)*invariance;
	}

	return (perfect_triangle_invariance)*(1/invariance);
}

double get_scale_inv(const Mat& src){

	double mu_00 = get_central_moment(src, 0,0);
	double mu_20 = get_central_moment(src, 2,0);
	double mu_02 = get_central_moment(src, 0,2);

	return ((mu_20+mu_02)/pow(mu_00,2));
}

void fill_circle(const Mat& src, Mat& dst){
	
	enum fill_state {
		NO_FILL,
		EDGE_FOUND,
		FILLING
	};
	fill_state state;
	Mat output = src.clone();
	Mat dummy = src.clone();
	for(int i = 0; i < src.rows; i++){
		state = NO_FILL;
		for(int j = 0; j < src.cols; j++){
			switch(state){
				case NO_FILL:
					if(src.at<char>(i,j) != 0){
						state = EDGE_FOUND;
					}
					break;
				case EDGE_FOUND:
					if(src.at<char>(i,j) == 0){
						dummy.at<char>(i,j) = 255;
						state = FILLING;
					}
					break;
				case FILLING:
					if(src.at<char>(i,j) == 0){
						dummy.at<char>(i,j) = 255;
					}
					else{
						state = EDGE_FOUND;
						dummy.row(i).copyTo(output.row(i));
					}
					break;
			}
		}
	}
	dst = output.clone();
}

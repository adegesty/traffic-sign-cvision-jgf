#ifndef MOMENT_H
#define MOMENT_H

#include <opencv2/opencv.hpp>
using namespace cv;

int get_moment(const Mat& src, int p, int q);
void get_covariance_eig_val(const Mat& src, double *eig_val_1, double *eig_val_2);
double get_central_moment(const Mat& src, int p, int q);
double get_invariance(const Mat& src);
double get_ellipticity(double invariance);
double get_rectangularity(double invariance);
double get_triangularity(double invariance);

#endif

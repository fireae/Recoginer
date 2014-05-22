#ifndef _SHAPE_CONTEXT_HPP_
#define _SHAPE_CONTEXT_HPP_
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void getShapeContextPoint(cv::Mat &img, cv::Point &center, Mat &descriptor,
	int num_rings, int num_wedges, double factor, double sigma, bool normalize = false,
	bool take_sqrt = false);

void getShapeContextImage(cv::Mat &image, vector<vector<double> > &descriptor,
	int num_rings, int num_wedges, double factor, double sigma, bool normalize = false,
	bool take_sqrt = false);


#endif //_SHAPE_CONTEXT_HPP_
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

void get2PointMatchCost(vector<double> &hi, vector<double> &hj, double &C);

void getMatchCost( vector<vector<double> >&p, vector<vector<double> >&q, Mat& C);

void getMatchCost( vector<vector<double> >&p, vector<vector<double> >&q, vector<vector<double > >& C);

void getBestMatch(vector< vector<double> > &C, vector<vector<int> > &result);

void getBestMatch(Mat &C, DMatch & match);

#endif //_SHAPE_CONTEXT_HPP_
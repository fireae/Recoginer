#ifndef _THIN_HPP_
#define _THIN_HPP_

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void thin(const Mat &image, Mat &dst, int iterations = 1);

#endif
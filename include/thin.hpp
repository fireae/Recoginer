#ifndef _THIN_HPP_
#define _THIN_HPP_

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// 这个函数用来细化二值CV_U8C1图像， 最后返回一个CV_U8C1图像
// 论文：A fast parallel algorithm for thinning digital patterns
// url:http://www.docin.com/p-609578135.html
void thin(const Mat &image, Mat &dst, int iterations = 1);

#endif
#ifndef _THIN_HPP_
#define _THIN_HPP_

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// �����������ϸ����ֵCV_U8C1ͼ�� ��󷵻�һ��CV_U8C1ͼ��
// ���ģ�A fast parallel algorithm for thinning digital patterns
// url:http://www.docin.com/p-609578135.html
void thin(const Mat &image, Mat &dst, int iterations = 1);

#endif
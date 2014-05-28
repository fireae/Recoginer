#ifndef _LOG_HPP_
#define _LOG_HPP_

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <stdarg.h>
using namespace cv;

namespace msg
{

void showImage( const char* winName, cv::Mat& image );

void debug(const char* fmt, ...);

int info(const char* fmt, ...);


}

#endif
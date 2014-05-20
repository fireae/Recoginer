#include "log.hpp"

namespace msg
{

void showImage( const char* winName, cv::Mat& image )
{
#if defined(DEBUG)
	cv::namedWindow(winName);
	cv::imshow(winName, image);
	cv::waitKey(0);
#endif

}

void debug(const char* fmt, ...)
{
#if defined(DEBUG)
	va_list ap;
	va_start(ap, fmt);
	char buf[256];

	vsnprintf(buf, 256, fmt, ap);
	va_end(ap);
#endif
}

int info(const char* fmt, ...)
{

	va_list ap;
	va_start(ap, fmt);
	char buf[256];

	int cnt = vsnprintf(buf, 256, fmt, ap);
	va_end(ap);
	return cnt;
}



} // namespace msg
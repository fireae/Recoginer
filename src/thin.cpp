#include "thin.hpp"


// uchar* p[10],代表10个数字，p[0] 代表sum 即2-9的和， p[1]代表2-9的关系和，p[2-9]代表围绕中心点的8个点
static void getKernel(Mat &img, int row_idx, int col_idx, uchar* p)
{
	int cols = img.cols;
	int rows = img.rows;

	if (col_idx <= 0 || col_idx > cols-1 ||
		row_idx <= 0 || row_idx > rows - 1)
		return;

	uchar *data0 = img.ptr<uchar>(row_idx - 1);
	uchar *data = img.ptr<uchar>(row_idx);
	uchar *data1 = img.ptr<uchar>(row_idx+1);
			
	if (data[col_idx] > 0)
	{
		p[2] = (row_idx==0) ? 0 : data0[col_idx];
		p[3] = (col_idx==cols-1) ? 0 : data0[col_idx+1];
		p[4] = (col_idx==cols-1)?0:data[col_idx+1];
		p[5] = (row_idx==rows-1||col_idx == cols-1)?0:data1[col_idx+1];
		p[6] = (row_idx==rows-1)?0:data1[col_idx];
		p[7] = (row_idx==rows-1||col_idx==0)?0:data1[col_idx-1];
		p[8] = (col_idx==0)?0:data[col_idx-1];
		p[9] = (row_idx==0||col_idx==0)?0:data0[col_idx-1];

		int sum = 0;
		for (int k = 2; k <= 9; k++)
		{
			sum += p[k];
			if (k == 9)
			{
				if (p[9] == 0 && p[2] > 0)
					p[1]++;
			}
			else 
			{
				if (p[k] == 0 && p[k+1] > 0)
					p[1]++;
			}	
		}
		p[0] = sum;
			
	} //if (data[j] > 0)
}

// 这个函数用来细化二值CV_U8C1图像， 最后返回一个CV_U8C1图像
// 论文：A fast parallel algorithm for thinning digital patterns
// url:http://www.docin.com/p-609578135.html

void thin(const Mat &image, Mat &dst, int iterations)
{
	threshold(image, image, 1, 1, CV_THRESH_BINARY);
	dst = image.clone();
	int rows = image.rows;
	int cols = image.cols;
	uchar p[10] = {0};
	for (int it_idx = 0; it_idx < iterations; it_idx++) 
	{
		Mat img_temp = dst.clone();
		for (int i = 1; i < rows-1; i++)
		{
			for (int j = 1; j < cols-1; j++)
			{
				memset(p, 0, 10);
				getKernel(img_temp, i, j, p);
				if ((p[0] > 1 && p[0] < 7) &&
					(p[1] == 1) &&
					(!(p[2] * p[4] * p[6])) &&
					(!(p[4] * p[6] * p[8]))
					)
				{
					dst.at<uchar>(i, j) = 0;
				}
			}//for (int j = 1; j < cols-1; j++)
		}//for (int j = 1; j < rows-1; j++)

		img_temp.release();
		img_temp = dst.clone();

		for (int i = 1; i < rows-1; i++)
		{
			for (int j = 1; j < cols-1; j++)
			{
				memset(p, 0, 10);
				getKernel(img_temp, i, j, p);
				if ((p[0] > 1 && p[0] < 7) &&
					(p[1] == 1) &&
					(!(p[2] * p[4] * p[8])) &&
					(!(p[2] * p[6] * p[8]))
					)
				{
					dst.at<uchar>(i, j) = 0;
				}
			}//for (int j = 1; j < cols-1; j++)
		}//for (int j = 1; j < rows-1; j++)


	}//for (int it_idx = 0; it_idx < iterations; it_idx++)

	threshold(dst, dst, 0, 255, CV_THRESH_BINARY);
}
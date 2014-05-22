#include "thin.hpp"


void thin(const Mat &image, Mat &dst, int iterations)
{
	dst = image.clone();
	int rows = image.rows;
	int cols = image.cols;
	uchar ap, p2, p3, p4, p5, p6, p7, p8, p9;
	uchar p[10] = {0};
	for (int it_idx = 0; it_idx < iterations; it_idx++) 
	{
		Mat img_temp = dst.clone();

		for (int i = 1; i < rows-1; i++)
		{
			uchar *data0 = img_temp.ptr<uchar>(i - 1);
			uchar *data = img_temp.ptr<uchar>(i);
			uchar *data1 = img_temp.ptr<uchar>(i+1);
			for (int j = 1; j < cols-1; j++)
			{
				if (data[j] > 0)
				{
					//ap = 0;
					p[2] = (i==0) ? 0 : data0[j];
					p[3] = (j=cols-1) ? 0 : data0[j+1];
					p[4] = (j==cols-1)?0:data[j+1];
					p[5] = (i==rows-1||j == cols-1)?0:data1[j+1];
					p[6] = (i==rows-1)?0:data1[j];
					p[7] = (i==rows-1||j==0)?0:data1[j-1];
					p[8] = (j==0)?0:data[j-1];
					p[9] = (i==0||j==0)?0:data0[j-1];

					int sum = 0;
					for (int k = 2; k <= 9; k++)
					{
						sum += p[k];
						if (k == 9)
						{
							if (p[9] == 0 && p[2] == 1)
								p[0]++;
						}
						else 
						{
							if (p[k] == 0 && p[k+1] == 1)
								p[0]++;
						}	
					}

					if (sum > 1 && sum < 7)
					{
						if (p[0] == 1)
						{
							if (!(p[2] && p[4] && p[6]))
							{
								if (!(p[4] && p[6] && p[8]))
								{
									dst.at<uchar>(i, j) = 0;
								}
							}
						}
					}
				}
			}
		}

	}
}
#include "shapecontext.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void getShapeContextPoint(cv::Mat &image, cv::Point &center, vector<double> &descriptor,
	int num_rings, int num_wedges, double factor, double sigma, bool normalize,
	bool take_sqrt)
{
	 /* Compute the radius of the log-polar circle */
	double radius = std::pow(factor, num_rings) - 1;
	int rad_int = cvRound(cvCeil(radius));
	int feature_size = num_rings * num_wedges;

	vector<double> bins(feature_size);
	vector<double> weights(feature_size);
	descriptor.resize(feature_size);

	double inv_log_factor = 1.0/ std::log(factor);
	double inv_2pi = 1.0/(2.0*CV_PI);

	int dx = 0, dy = 0;
	Size img_sz = image.size();
	for (dy = -rad_int; dy <= rad_int; dy++) 
	{
		for (dx = -rad_int; dx <= rad_int; dx++)
		{
			int xi = center.x + dx;
			int yi = center.y + dy;

			int conf;
			if (xi < 0 || xi >= img_sz.height ||
				yi < 0 || yi >= img_sz.width)
			{
				conf = 0;
			}
			else 
			{
				//std::cout << "i: " << xi << "j: " << yi << std::endl;
				conf = image.at<uchar>(xi, yi);
			}

			/* Compute the bin for this pixel */
			double rho = std::sqrt(double(dx * dx + dy * dy));
			double theta = std::atan2((double)dy, (double)dx);
			
			if (theta < 0.0)
				theta += 2.0 * CV_PI;

			if (rho >= radius)
				continue;

			/* Compute the floor of the bucket number */
			double R = log(rho + 1.0) * inv_log_factor; // / log(factor);
			double W = num_wedges * theta * inv_2pi;
			int Rf = cvRound(cvFloor(R));
			int Wf = cvRound(cvFloor(W));

			double t = R - Rf;
			double u = W - Wf;

			double wt0 = (1.0 - t) * (1.0 - u);
			double wt1 = (1.0 - t) * u;
			double wt2 = t * (1.0 - u);
			double wt3 = t * u;

			if (Rf < 0 || Rf >= num_rings || Wf < 0 || Wf >= num_wedges)
				continue;

			bins[Rf * num_wedges + Wf] += conf*wt0;
			bins[Rf * num_wedges + (Wf+1)%num_wedges] += conf * wt1;

			weights[Rf * num_wedges + Wf] += wt0;
			weights[Rf * num_wedges + (Wf+1) % num_wedges] += wt1;

			if (Rf + 1 < num_rings) 
			{
				bins[(Rf+1) * num_wedges + Wf] += conf * wt2;
				bins[(Rf+1) * num_wedges + (Wf+1) % num_wedges] += conf * wt3;

				weights[(Rf+1) * num_wedges + Wf] += wt2;
				weights[(Rf+1) * num_wedges + (Wf+1) % num_wedges] += wt3;
			}
		}
	}

	for (int i = 0; i < feature_size; i++) 
	{
		if (weights[i] > 0.0)
			descriptor[i] = bins[i] / weights[i];

		else 
			descriptor[i] = 0.0;
	}

	if (normalize) 
	{
		/* Normalize histogram */

		double mass = 0.0;
		for (int i = 0; i < feature_size; i++) 
		{
			mass += descriptor[i];
		}

		if (mass > 0.0) 
		{
			for (int i = 0; i < feature_size; i++) 
			{
				descriptor[i] /= mass;

                if (take_sqrt)
                    descriptor[i] = sqrt(descriptor[i]);// descriptor[i] *= 2048.0;
			}
	    }
	}

}

void getShapeContextImage(cv::Mat &image, vector<vector<double> > &descriptor,
	int num_rings, int num_wedges, double factor, double sigma, bool normalize,
	bool take_sqrt)
{
	int rows = image.rows;
	int cols = image.cols;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			uchar c = (uchar)image.at<uchar>(i, j);
			if (c != 0)
			{
				vector<double> desc;
				getShapeContextPoint(image, Point(i,j), desc, num_rings, num_wedges, 
					factor, sigma, normalize, take_sqrt);
				descriptor.push_back(desc);
			}
		}
	}
}

void get2PointMatchCost(vector<double> &hi, vector<double> &hj, double &C)
{
	if (hi.size() != hj.size())
		return;

	double sum = 0.0;
	int sz = hi.size();
	for (int k = 0 ; k < sz; k++)
	{
		if (hi[k] + hj[k] < 0.0000001)
			sum += 0.0;
		else
		{
			sum += (hi[k] - hj[k])*(hi[k] - hj[k])/(hi[k] + hj[k]);
		}
	}

	C = 0.5 * sum;
}

void getMatchCost( vector<vector<double> >&p, vector<vector<double> >&q, Mat& C)
{
	int max_len = std::max(p.size(), q.size());
	C = Mat::zeros(Size(max_len, max_len), CV_64FC1);

	for (int i = 0; i < p.size(); i++)
	{
		vector<double> &hi = p.at(i);
		for (int j = 0; j < q.size(); j++)
		{
			vector<double> &hj = q.at(j);
			double c;
			get2PointMatchCost(hi, hj, c);

			C.at<double>(i, j) = c;
		}
	}
}

void getMatchCost( vector<vector<double> >&p, vector<vector<double> >&q, vector<vector<double > >& C)
{
	int max_size = std::max(p.size(), q.size());
	for (int i = 0; i < max_size; i++)
	{
		vector<double> ci;
		for (int j = 0; j < max_size; j++)
		{
			ci.push_back(0.0);
		}
		C.push_back(ci);
	}

	for (int i = 0; i < p.size(); i++)
	{
		vector<double> &hi = p.at(i);
		vector<double> &ci = C[i];
		for (int j = 0; j < q.size(); j++)
		{
			vector<double> &hj = q.at(j);
			double c;
			get2PointMatchCost(hi, hj, c);
			ci[j] = c;
		}
	}
}

void getBestMatch(vector< vector<double> > &C, vector<vector<int> > &result)
{
	//HungarianMethod method;
	//method.CallHM(C, result);
	//method.OutResult();
}

///////// end of line ////////

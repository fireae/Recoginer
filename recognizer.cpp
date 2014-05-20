#include "recognizer.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>
#include "os.hpp"
#include "log.hpp"

using namespace os;
using namespace std;
using namespace cv;

Recoginzer::Recoginzer()
{
}

Recoginzer::~Recoginzer()
{
}

bool Recoginzer::train(cv::Mat &trainData, std::vector<int> labels)
{
	return true;
}

float Recoginzer::predict( cv::Mat &sample )
{
	return 0.0;
}


void Recoginzer::save(const char *filename)
{
}

void Recoginzer::load(const char *filename)
{	
}

bool SVMRecognizer::train(cv::Mat &trainData, std::vector<int> labels)
{
	Mat mLabels(labels);
	return svm.train(trainData, mLabels, Mat(), Mat(), params);
}

float SVMRecognizer::predict( cv::Mat &sample )
{
	return svm.predict(sample);
}

static void bincount(cv::Mat& x, cv::Mat& weight, int minlength, vector<float>& vechist)
{
	int rows = x.rows;
	int cols = x.cols;
	uchar *pu;
	float *pf;
	vechist.resize(minlength);
	for (int i = 0; i < rows; i++){
		pu = x.ptr<uchar>(i);
		pf = weight.ptr<float>(i);

		for (int j = 0; j< cols; j++){
			uchar u = *pu ++;
			float f = *pf ++;
			if (u < minlength)
				vechist[u] += f;
		}
	}
}

static void preprocess_hog(Mat& img, vector<float>& vecHist)
{
	cv::Mat grad_x, grad_y;
	Sobel(img, grad_x, CV_32F, 1, 0);
	Sobel(img, grad_y, CV_32F, 0, 1);

	Mat magnitude, angle;
	cartToPolar(grad_x, grad_y, magnitude, angle, false);
	angle = angle*16/(2*3.1415926);
	Mat hAngle = angle.reshape(0,1);
	Mat hUAngle;
	hAngle.convertTo(hUAngle, CV_8UC1);
	Mat hMagnitude = magnitude.reshape(0,1);
	vector<float > vechist1;

	bincount(hUAngle, hMagnitude, 16, vechist1);

	Mat h(vechist1);
	Scalar s = sum(h);
	h /= s[0];
	Mat hs;
	cv::sqrt(h, hs);
	normalize(hs, hs);


	MatIterator_<float> it = hs.begin<float>();
	MatIterator_<float> end = hs.end<float>();
	for (; it != end; it++)
		vecHist.push_back(*it);
}


void FeatureMaker::getFeatures(const cv::Mat &img, cv::Mat &feature)
{
	vector<Mat> vecImg(4);
	vecImg[0] = img(Rect(0,0, 10,10));
	vecImg[1] = img(Rect(10,0,10,10));
	vecImg[2] = img(Rect(0,10,10,10));
	vecImg[3] = img(Rect(10,10,10,10));

	feature = Mat::zeros(Size(1, 64), CV_32F);
	for (int i =0; i < 4;i++)
	{
		vector<float> vh;
		preprocess_hog(vecImg[i], vh);
		for (int j = 0; j < vh.size(); j++)
			feature.push_back(vh[j]);
	}

}


void FeatureMaker::makeSample(std::string trainDataPath, 
	cv::Mat &samples, std::vector<float> &labels)
{
	vector<string> files;
	os::listdir(trainDataPath, files);
	for (int i = 0; i < files.size(); i++) 
	{
		string dirPath;
		os::path::join(trainDataPath, files[i], dirPath);

		if (os::isdir(dirPath)) 
		{
			vector<string> imgFiles;
			os::listdir(dirPath, imgFiles);
			string imgPath;
			for (int j = 0; j < imgFiles.size(); j++)
			{
				os::path::join(dirPath, imgFiles[i], imgPath);
				cv::Mat img = cv::imread(imgPath);
				msg::info("%s", imgPath.c_str());
				Mat feature;
				getFeatures(img, feature);

				features_.push_back(feature);
				float label = atof(files[i].c_str());
				labels_.push_back(label);
			}
		}
	}

	Size s = features_.size();
	features_ = features_.reshape(0, s.width*s.height/ 128);
	s = features_.size();
	samples = features_;
	labels = labels_;
}

void FeatureMaker::save(const char *filename)
{
	FileStorage fs(filename, FileStorage::WRITE);
	if (!fs.isOpened())
		return;
	fs << "features" << features_;
	
	fs << "labels" << labels_;

}

void FeatureMaker::load(const char *filename)
{
	FileStorage fs(filename, FileStorage::READ);
	if (!fs.isOpened())
		return;
	fs["features"] >> features_;
	
	FileNode fn = fs["labels"];
	FileNodeIterator it = fn.begin(), it_end = fn.end();
	for (; it != it_end; ++it) 
	{
		labels_.push_back(int(*it));
	}
	
}

// end of file

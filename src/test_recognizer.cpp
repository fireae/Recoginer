#include <windows.h>
#include "os.hpp"
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "recognizer.hpp"
#include "shapecontext.hpp"
#include "thin.hpp"
#include "munkres.h"

using namespace cv;
using namespace std;
using namespace os;

void getFeature(Mat &img, Mat &feature)
{
	feature = img.reshape(1, 1);
}

int main(int argc, char *argv[])
{
#if 1
	std::vector<string> files;
	string dir_path = "D:\\Pictures\\bill5\\2"; 
	os::listdir(dir_path, files);

	Mat img1 = cv::imread("D:\\Pictures\\bill5\\8\\86.bmp", 0);
	
	Mat bin1;
	cv::threshold(img1, bin1, 100, 255, CV_THRESH_BINARY);
	Mat thin1;
	thin(bin1, thin1, 7);

	vector< vector<double> > desc1;
	getShapeContextImage(thin1, desc1, 5, 12, 1.6, 0.0, true, true);
	
	for (int i = 0; i < files.size(); i++)
	{
		string img_path;
		os::path::join(dir_path, files[i], img_path);
		Mat img2 = cv::imread(img_path, 0);
	
		Mat bin2;
		cv::threshold(img2, bin2, 100, 255, CV_THRESH_BINARY);
		Mat thin2;
		thin(bin2, thin2, 7);
		vector< vector<double> > desc2;
		getShapeContextImage(thin2, desc2, 5, 12, 1.6, 0.0, true, true);
		Mat C;
		getMatchCost(desc1, desc2, C);

		Munkres munk;
		vector<int> result;
		double res_val;
		munk.solve(C, result, res_val);
		double avg = res_val / (desc1.size() + desc2.size());
		cout << "idx: " << i << " res value: " << res_val << ", avg is: " << avg << endl;
	}
	Mat img2 = cv::imread("D:\\Pictures\\bill5\\4\\140.jpg", 0);
	
	Mat bin2;
	cv::threshold(img2, bin2, 100, 255, CV_THRESH_BINARY);
	Mat thin2;
	thin(bin2, thin2, 7);
	vector< vector<double> > desc2;
	getShapeContextImage(thin2, desc2, 5, 12, 1.6, 0.0, true, true);
	Mat C;
	getMatchCost(desc1, desc2, C);

	Munkres munk;
	vector<int> result;
	double res_val;
	munk.solve(C, result, res_val);
	//cout << "C" << C << endl;

	//getBestMatch(C, result);
	cout << "res value: " << res_val << endl;
	double pf = std::pow(2.0, 5);
	cout << pf << endl;
	int b;
	cin >> b;
	return 1;
#endif

#if 1

	struct FeatureMaker maker;
	Mat samples;
	vector<float> labels;
	maker.makeSample("D:\\Pictures\\bill5", samples, labels);
	maker.save("d:\\a.xml");
	cout << "ok" << endl;

	Mat fsamples;
	samples.convertTo(fsamples, CV_32FC1);
	//train
	SVMRecognizer svmReg;
	svmReg.train(fsamples, labels);
	svmReg.save("d:\\a.dat");

	cv::Mat test_img = cv::imread("d:\\Downloads\\132.bmp", 0);
	
	Preprecessor pre;
	Mat dst;
	pre.preprocess(test_img, dst);
	cv::imshow("g", dst);
	cv::waitKey();
	cv::Mat test_feature;
	maker.getFeatures(dst, test_feature);
	cv::Mat ftest_feature;
	test_feature.convertTo(ftest_feature, CV_32FC1);
	float f = svmReg.predict(ftest_feature);
	cout << f;
#endif

#if 0
	struct FeatureMaker maker;
	Mat samples;
	vector<float> labels;
	maker.makeSample("D:\\Pictures\\bill5", samples, labels);
	maker.save("d:\\a.xml");
	cout << "ok" << endl;
	Mat trainClasses(labels);
	Mat fsamples;
	samples.convertTo(fsamples, CV_32FC1);
	CvKNearest knn(fsamples, trainClasses);

	cv::Mat test_img = cv::imread("d:\\Downloads\\2.bmp", 0);
	Preprecessor pre;
	Mat dst;
	pre.preprocess(test_img, dst);
	Mat test_feature;
	maker.getFeatures(dst, test_feature);
	Mat nearests;
	Mat result;
	Mat ftest;
	test_feature.convertTo(ftest, CV_32FC1);
	double response = knn.find_nearest(ftest,20,result, nearests, Mat());
#endif

	int a;
	cin >> a;
	return 0;
}





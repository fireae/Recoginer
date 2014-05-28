#include <windows.h>
#include "os.hpp"
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "recognizer.hpp"
#include "shapecontext.hpp"
#include "thin.hpp"
using namespace cv;
using namespace std;
using namespace os;

int main(int argc, char *argv[])
{
	for (int i = 0; i < 10; i++)
	{
		char szNum[100];
		sprintf(szNum, "%s/%d", "D:\\Pictures\\num_samples", i);
		vector<string> files;
		os::listdir(szNum, files);

		for (int j = 0; j < files.size(); j++)
		{
			string img_path;
			os::path::join(szNum, files[j], img_path);
			Mat img2 = cv::imread(img_path, 0);
			cv::medianBlur(img2, img2, 3);
			Mat thin1;
			Mat bin2;
			thin(img2, thin1, 3);
			
			cv::imshow("b", thin1);
			cv::waitKey();
		}
	}


	Mat img2 = cv::imread("d:\\0007.bmp", 0);
	
	Mat bin2;
	cv::threshold(img2, bin2, 100, 255, CV_THRESH_BINARY);
	Mat thin1;
	thin(bin2, thin1, 7);


	vector< vector<double> > descriptor;
	getShapeContextImage(thin1, descriptor, 5, 12, 2, 0.0, true, true);
	
	Mat img21 = cv::imread("d:\\0448.bmp", 0);
	
	Mat bin21;
	cv::threshold(img21, bin21, 100, 255, CV_THRESH_BINARY);
	Mat thin11;
	thin(bin21, thin11, 7);
	vector< vector<double> > desc1;
	getShapeContextImage(thin1, desc1, 5, 12, 2, 0.0, true, true);
	vector<vector<double > > C;
	getMatchCost(descriptor, desc1, C);

	vector<vector<int> > result;
	getBestMatch(C, result);
	
	double pf = std::pow(2.0, 5);
	cout << pf << endl;
	return 1;

	vector<string> files;

	struct FeatureMaker maker;
	Mat samples;
	vector<float> labels;
	maker.makeSample("D:\\Pictures\\num_samples", samples, labels);
	maker.save("d:\\a.xml");
	cout << "ok" << endl;


	//train
	SVMRecognizer svmReg;
	svmReg.train(samples, labels);
	svmReg.save("d:\\a.dat");

	cv::Mat test_img = cv::imread("d:\\Downloads\\4.bmp", 0);
	
	Preprecessor pre;
	Mat dst;
	pre.preprocess(test_img, dst);

	cv::Mat test_feature;
	maker.getFeatures(dst, test_feature);
	float f = svmReg.predict(test_feature);
	cout << f;
	int a;
	cin >> a;
	return 0;
}




#include <windows.h>
#include "os.hpp"
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "recognizer.hpp"
using namespace cv;
using namespace std;
using namespace os;

int main(int argc, char *argv[])
{
	vector<string> files;
	os::listdir("d:\\testdir", files);
	for (int i = 0; i < files.size(); i++)
	{
		std::cout << files[i].c_str() << std::endl;
		string str_path = "d:\\testdir\\" + files[i];
		cout << str_path << endl;
		if (os::isfile(str_path.c_str()))
			cout << "true" << endl;
	}

	//Mat img = Mat::eye(Size(20, 20), CV_8UC1); 
	Mat img = cv::imread("D:\\Pictures\\0\\0.jpg", 0);
	string path, file;
	char file_path[] = "D:\\Pictures\\0\\0.jpg";
	os::path::split(file_path, path, file);
	string new_path;
	os::path::join(path, file, new_path);
	cout << "new path  " << new_path<< endl;
	struct FeatureMaker maker;
	
	Mat samples;
	vector<float> labels;
	maker.makeSample("D:\\Pictures\\num_samples", samples, labels);
	maker.save("d:\\a.xml");
	cout << "ok" << endl;

	//train
	CvSVM svm;
	CvSVMParams params;
	params.svm_type    = CvSVM::C_SVC;
	params.kernel_type = CvSVM::RBF;
	params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 0.2);
	Mat mLabels(labels);
	svm.train(samples, mLabels, Mat(), Mat(), params);

	cv::Mat test_img = cv::imread("E:\\workplace\\python\\test_hu\\7.jpg", 0);
	
	Mat bin;
	threshold(test_img, bin, 100, 255, CV_THRESH_BINARY);

	Size s = bin.size();
	int mx = max(s.width, s.height);


	cv::Mat m = Mat::eye(Size(3, 2), CV_32F);
	m.at<float>(0, 2) = mx/2.0 - s.width/2.0;
	m.at<float>(1, 2) = mx/2.0 - s.height/2.0;
	Mat affine;
	warpAffine(bin, affine, m, Size(mx, mx));
	Mat res;
	resize(affine, res, Size(20,20));
	cv::Mat test_feature;
	maker.getFeatures(res, test_feature);
	float f = svm.predict(test_feature);
	cout << f;
	int a;
	cin >> a;
	return 0;
}





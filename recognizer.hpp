#ifndef _RECOGNIZER_HPP_
#define _RECOGNIZER_HPP_

#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


// ��ѵ��ͼ�������ͼ��ת���ɶ�ֵͼ
// ��ѵ�������������ÿ������Ӧ���ļ���
struct Preprecessor 
{
	void preprocess(const cv::Mat &image, cv::Mat &dst);

};


// ��ȡ��������������ֵ
// ����������������ֵ��װ��ѵ�����ݣ�������
struct FeatureMaker
{
	void getFeatures(const cv::Mat &img, cv::Mat &feature);

	void makeSample(std::string trainDataPath,
			cv::Mat &samples, std::vector<float> &labels);

	void save(const char *filename);
	void load(const char *filename);

	Mat features_;
	vector<float> labels_;

};


class Recoginzer {
public:
	Recoginzer();

	virtual ~Recoginzer();

	virtual bool train(cv::Mat &trainData, std::vector<int> labels);

	virtual float predict( cv::Mat &sample );

private: 
	void save(const char *filename);

	void load(const char *filename);

};


class SVMRecognizer : public Recoginzer
{
public:
	SVMRecognizer()
	{
		params.svm_type    = CvSVM::C_SVC;
 		params.C           = 0.1;
 		params.kernel_type = CvSVM::LINEAR;
 		params.term_crit   = TermCriteria(CV_TERMCRIT_ITER, (int)1000, 1e-4);
	}
	SVMRecognizer(double C, double gamma)
	{
 		params.svm_type    = CvSVM::C_SVC;
 		params.C           = C;
 		params.kernel_type = CvSVM::LINEAR;
 		params.term_crit   = TermCriteria(CV_TERMCRIT_ITER, (int)1e7, gamma);
	}

	bool train(cv::Mat &trainData, std::vector<int> labels);

	float predict( cv::Mat &sample );

private:
	CvSVM svm;
	CvSVMParams params;
};

#endif //_RECOGNIZER_HPP_
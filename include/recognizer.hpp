#ifndef _RECOGNIZER_HPP_
#define _RECOGNIZER_HPP_

#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


// 将训练图像或样本图像转化成二值图
// 将训练样本分类放入每个类别对应的文件夹
struct Preprecessor 
{
	void preprocess(const cv::Mat &image, cv::Mat &dst);

};


// 提取单个样本的特征值
// 将所有样本的特征值组装成训练数据，并分类
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

	virtual void save(const char *filename);

	virtual void load(const char *filename);

};


class SVMRecognizer : public Recoginzer
{
public:
	SVMRecognizer()
	{
		params.svm_type    = CvSVM::C_SVC;
 		params.kernel_type = CvSVM::RBF;
 		params.term_crit   = TermCriteria(CV_TERMCRIT_ITER, (int)1000, 0.01);
	}
	SVMRecognizer(double C,  double gamma, int maxCnt, double esp)
	{
 		params.svm_type    = CvSVM::C_SVC;
 		params.C           = C;
 		params.kernel_type = CvSVM::RBF;
		params.gamma = gamma;
 		params.term_crit   = TermCriteria(CV_TERMCRIT_ITER, maxCnt, esp);
	}

	bool train(cv::Mat &trainData, std::vector<float> &labels);

	float predict( cv::Mat &sample );

	void save(const char *filename);

	void load(const char *filename);
private:
	CvSVM svm;
	CvSVMParams params;
};

#endif //_RECOGNIZER_HPP_
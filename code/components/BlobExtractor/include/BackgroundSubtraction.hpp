#ifndef BACKGROUND_SUBTRACTION_HPP
#define BACKGROUND_SUBTRACTION_HPP

#include <opencv2/opencv.hpp>


class BackgroundSubtraction
{
public:
	
	enum SubtractorType
	{
		DEPTH_THRESHOLD,
		MOG2,
		KNN
	};

	void Subtract(cv::Mat& src, cv::Mat& dst);

	void SetDepthThreshold(int threshold);

	BackgroundSubtraction(BackgroundSubtraction::SubtractorType type);

	BackgroundSubtraction(BackgroundSubtraction::SubtractorType type, int history, int varThreshold, bool shadows);

	void Apply(cv::Mat& src, cv::Mat &dst);

private:
	bool m_backgroundSet;

	int m_threshold;

	SubtractorType m_type;

	//cv::Ptr<cv::BackgroundSubtractor> m_bgSubtractor;
	cv::Mat m_background;
};

#endif

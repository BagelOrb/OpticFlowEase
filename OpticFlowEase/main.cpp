/*
* main.cpp
*
*  Created on: 10 Nov 2014
*      Author: coert
*/
#include "stdafx.h"

#include <memory>

#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>

#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#ifdef __linux__
#include <GL/glut.h>
#include <GL/glu.h>
#endif

using namespace cv;
using namespace std;

void showFlow(Mat flow)
{
	Mat xy[2];
	split(flow, xy);

	//calculate angle and magnitude
	Mat magnitude, angle;
	cartToPolar(xy[0], xy[1], magnitude, angle, true);

	//translate magnitude to range [0;1]
	double mag_max;
	minMaxLoc(magnitude, 0, &mag_max);
	magnitude.convertTo(magnitude, -1, 1.0 / mag_max);

	//build hsv image
	Mat _hsv[3], hsv;
	_hsv[0] = angle;
	_hsv[1] = Mat::ones(angle.size(), CV_32F);
	_hsv[2] = magnitude;
	merge(_hsv, 3, hsv);

	//convert to BGR and show
	Mat bgr;//CV_32FC3 matrix
	cvtColor(hsv, bgr, COLOR_HSV2BGR);
	imshow("Flow", bgr);

};

int main(int argc, char** argv)
{
	//cv::Mat penguins = cv::imread("C:\\Users\\TK\\Documents\\Visuals\\OpticFlowEase\\data\\Penguins.jpg");

	int layers = 3;
	int averaging_block_size = 2;
	int max_flow = 4;




	VideoCapture video = VideoCapture("C:\\Users\\TK\\Documents\\Visuals\\OpticFlowEase\\data\\video.avi");
	assert(video.isOpened());
	
	Mat frame;
	Mat grey_frame;
	Mat last_frame;
	Mat last_grey_frame;

	Mat flow;

	video.read(last_frame);
	cvtColor(last_frame, last_grey_frame, CV_RGB2GRAY);

	cv::namedWindow("Video", CV_WINDOW_KEEPRATIO);
	cv::namedWindow("Flow", CV_WINDOW_KEEPRATIO);


	while (true)
	{
		video.read(frame);
		cvtColor(frame, grey_frame, CV_RGB2GRAY);
		cv::imshow("Video", frame);
		calcOpticalFlowSF(last_frame, frame, flow, layers, averaging_block_size, max_flow);
		//showFlow(flow);




		last_frame = frame;
		last_grey_frame = grey_frame;
		char key = cv::waitKey(10);
		if (key == 27) exit(EXIT_SUCCESS);
	}

	return EXIT_SUCCESS;
}

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//  #include <opencv2/tracking.hpp>
 #include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;



Mat LaplaceGaussian(Mat &image);

Mat Threshold(Mat &image);
Mat Gaussian_Blur(Mat &image);
Mat Region_of_interest(Mat &src);




#endif 

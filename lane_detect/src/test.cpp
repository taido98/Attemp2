#include <iostream>
#include "ultil.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;
int main(){
    Mat img = imread('~/Documents/catkin_ws/src/images/shaddow.png');
    imshow('image', img);
    return 0;
}
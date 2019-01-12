#include <iostream>
// #include <cv.h>
// #include <highgui.h>
#include "utils.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;


// Mat Region_of_interest(Mat &src){

//      Mat mask = cvCreateMat(240, 320, CV_8UC1);
//     // Create black image with the same size as the original
//     for(int i=0; i<mask.cols; i++)
//     for(int j=0; j<mask.rows; j++)
//         mask.at<uchar>(Point(i,j)) = 0;
    
//     // Create Polygon from vertices
//     vector<Point> ROI_Poly;
//     vector<Point> ROI_Vertices;
//     ROI_Vertices.push_back(Point(30,240));
//     ROI_Vertices.push_back(Point(60,80));
//     ROI_Vertices.push_back(Point(250,80));
//     ROI_Vertices.push_back(Point(320,240));
//     approxPolyDP(ROI_Vertices, ROI_Poly, 1.0, true);
    
//     // Fill polygon white
//     fillConvexPoly(mask, &ROI_Poly[0], ROI_Poly.size(), 255, 8, 0);                 
    
//     // Create new image for result storage
//     // Mat imageDest = cvCreateMat(480, 640, CV_8UC3);
//     return mask;
// }

int main(){
    // Mat img = imread("~/Documents/catkin_ws/src/images/shaddow.png",1);
    Mat img = imread("ngaba3.png");
    cout << img.type()<<endl;
    cv::Mat mask = cv::Mat::zeros(Size(320, 240), CV_8UC3);
    // Mat mask =  Threshold(img);
    imshow("image", mask);
    waitKey(0);  
    cout << "hello";
    return 0;
}
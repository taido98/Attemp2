#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

#include <opencv2/highgui.hpp>

#include "detectlane.h"
#include "carcontrol.h"
// #include <vector>

bool STREAM = true;

VideoCapture capture("video.avi");
DetectLane *detect;
CarControl *car;
int skipFrame = 1;
class State{
    public:

    private:
        double car_x ;
        double car_y;
        double car_s;
        double car_d;
        double end_path_s;
        double end_path_d;

        vector<double> ptsx;
        vector<double> ptsy;

        double cte;
        double speed;
        double angle;



};
void imageCallback(const sensor_msgs::ImageConstPtr &msg)
{

    cv_bridge::CvImagePtr cv_ptr;
    Mat out;
    try
    {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        // cv::imshow("View", cv_ptr->image);
        char k = (char)waitKey(1);
        if (k == 'q')
        {
            ros::shutdown();
        }
        detect->update(cv_ptr->image);
        // vector<Point> ()
        car->driverCar(detect->getLeftLane(), detect->getRightLane(), 50);
    }
    catch (cv_bridge::Exception &e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}

void videoProcess()
{
    Mat src;
    src = imread("/home/thanh/shaddow.png");
    imshow("View", src);
    detect->update(src);
    waitKey(0);
    
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "image_listener");
    cv::namedWindow("View");
    cv::namedWindow("Binary");
    cv::namedWindow("Threshold");
    cv::namedWindow("Bird View");
    cv::namedWindow("Lane Detect");
    
    if (STREAM)
    {
        detect = new DetectLane();
        car = new CarControl();
        cv::startWindowThread();

        ros::NodeHandle nh;
        image_transport::ImageTransport it(nh);
        image_transport::Subscriber sub = it.subscribe("Team1_image", 1, imageCallback);

        ros::spin();
    }
    else
    {
        cout <<"hello";
        videoProcess();
    }
    cv::destroyAllWindows();
}

#ifndef CARCONTROL_H
#define CARCONTROL_H
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <ros/ros.h>
#include "std_msgs/Float32.h"
#include "std_msgs/String.h"
#include <vector>
#include <math.h>

#include "detectlane.h"

using namespace std;
using namespace cv;

class CarControl 
{
public:
    CarControl();
    ~CarControl();
    void driverCar(const vector<Point> &left, const vector<Point> &right, float velocity);
    void Callback(const std_msgs::String::ConstPtr& msg);

private:
    float errorAngle(const Point &dst);
    ros::NodeHandle node_obj1;
    ros::NodeHandle node_obj2;
    ros::NodeHandle node_obj3;

    
    ros::Publisher steer_publisher;
    ros::Publisher speed_publisher;
    ros::Subscriber sub;
    Point carPos;
    String sign_name;

    float laneWidth = 40;

    float minVelocity = 10;
    float maxVelocity = 50;

    float preError;

    double kP;
    double kI;
    double kD;

    int t_kP;
    int t_kI;
    int t_kD;
};

#endif

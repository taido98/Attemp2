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
    void callBackSign(const std_msgs::String::ConstPtr& msg);
private:
    float errorAngle(const Point &dst);
    ros::NodeHandle node_obj1;
    ros::NodeHandle node_obj2;
    ros::NodeHandle node_obj3;
    
    ros::Publisher steer_publisher;
    ros::Publisher speed_publisher;
    ros::Subscriber sign_subcriber;
    Point carPos;

    float laneWidth = 40;
    bool BRAKE = false;
    bool FORWARD = true;
    float minVelocity = 10;
    float maxVelocity = 50;

    float preError;

    float kP;
    float kI;
    float kD;

    int t_kP;
    int t_kI;
    int t_kD;

    double speed = 60;
    double angle = 0;

    
    


    bool hasSign = false;
    bool violate_left = false;
    bool violate_right = false;

   
    double distance(double x1, double y1, double x2, double y2)
    {
        return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
    }

    double absolute(double x1, double y1)
    {
        return sqrt((x1)*(x1)+(y1)*(y1));
    }

    double cost_keep_lane(double dist_closest_front, double cost_sign, double obstack_location, double dist_to_sign);
    double cost_change_left(double dist_closest_leftFront, double dist_closest_leftback,double cost_sign, double obstack_location, double dist_to_sign);
    double cost_change_right(double dist_closest_rightFront, double dist_closest_rightback,double cost_sign, double obstack_location, double dist_to_sign);
    double cost_slow_down(double cost_);
};

#endif

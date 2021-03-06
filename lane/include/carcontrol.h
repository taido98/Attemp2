#ifndef CARCONTROL_H
#define CARCONTROL_H
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <ros/ros.h>
#include "std_msgs/Float32.h"
#include "std_msgs/String.h"
// #include "sign/SignMsg.h"
#include <vector>
#include <math.h>
#include "PID.h"
#include "detectlane.h"

using namespace std;
using namespace cv;
vector<int> SIGN ;//= {0,0,0,0,0,0,0,0,0,0};
class CarControl 
{
public:
    CarControl();
    ~CarControl();
    void driverCar(const vector<Point> &left, const vector<Point> &right, int &velocity);
    void driverCar2(const vector<Point> &left, const vector<Point> &right, int &velocity);
    // void callBackSign(const std_msgs::SignMsg::ConstPtr& msg);
    //  void callBackSign(const std_msgs::String::ConstPtr& msg);
     void CallbackSign(const std_msgs::String::ConstPtr& msg);
     void CallbackFraction(const std_msgs::Float32::ConstPtr& msg); 
     
    
private:
    float errorAngle(const Point &dst);
    ros::NodeHandle node_obj1;
    ros::NodeHandle node_obj2;
    ros::NodeHandle node_obj3;
    ros::NodeHandle node_obj4;
    
    ros::Publisher steer_publisher;
    ros::Publisher speed_publisher;
    ros::Subscriber sign_subcriber;
    ros::Subscriber fraction_subcriber;

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

    int t_kP = 2130;
    int t_kI = 770;
    int t_kD = 319;

    double speed = 60;
    double angle = 0;


    int time_forward = 10;
    int time_turn = 5;
    int turnAngle = 5; // Hard angle 5-50
    string sign;
    
    // double cost;
    string prestate;
    int index = 0;
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

    double cost_keep_lane(double cost_sign);
    double cost_change_left(double cost_turn_left);
    double cost_change_right(double cost_turn_right);
    double cost_slow_down(double cost_);
};

#endif

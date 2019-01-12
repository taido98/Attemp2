#include "carcontrol.h"
#include "PID.h"

CarControl::CarControl()
{

    cvNamedWindow("PID",1);
    cvCreateTrackbar("LowP", "PID", &t_kP, 10000);

    cvCreateTrackbar("LowI", "PID", &t_kI, 1000);
    // cvCreateTrackbar("HighI", "PID", &maxPID[1], 255);

    cvCreateTrackbar("LowD", "PID", &t_kD, 1000);
    // cvCreateTrackbar("HighD", "PID", &maxPID[2], 255);

    carPos.x = 120;
    carPos.y = 300;
    steer_publisher = node_obj1.advertise<std_msgs::Float32>("Team1_steerAngle",10);
    speed_publisher = node_obj2.advertise<std_msgs::Float32>("Team1_speed",10);
    sub = node_obj3.subscribe("sign_name", 1000, &CarControl::Callback,this);
    

}
//CarControl::~CarControl() {}



void CarControl::Callback(const std_msgs::String::ConstPtr& msg)
{
    sign_name = msg->data.c_str();
    ROS_INFO("I heard: [%s]",msg->data.c_str());  
    cout << "-------------------------------------------------";
}

float CarControl::errorAngle(const Point &dst)
{
    if (dst.x == carPos.x) return 0;
    if (dst.y == carPos.y) return (dst.x < carPos.x ? -90 : 90);
    
    kP = (double)t_kP/10000.0;
    kI = (double)t_kI/100.0;
    kD = (double)t_kD/100.0;

    double pi = acos(-1.0);
    double dx = dst.x - carPos.x;
    double dy = carPos.y - dst.y; 

    if (dx < 0) {
        PID pid;
        pid.Init(kP, kI, kD);
        pid.UpdateError(-dx);
        dx = pid.TotalError();
        return -atan(-dx/dy) * 180 / pi;
    }
        
    PID pid;
    pid.Init(kP, kI, kD);
    pid.UpdateError(dx);
    dx = pid.TotalError();
    return atan(-dx/dy) * 180 / pi;
}

void CarControl::driverCar(const vector<Point> &left, const vector<Point> &right, float velocity)
{
    int i = left.size() - 11;
    float error = preError;
    while (left[i] == DetectLane::null && right[i] == DetectLane::null) {
        i--;
        if (i < 0) return;
    }
    if (left[i] != DetectLane::null && right[i] !=  DetectLane::null)
    {
        error = errorAngle((left[i] + right[i]) / 2);
    } 
    else if (left[i] != DetectLane::null)
    {
        error = errorAngle(left[i] + Point(laneWidth / 2, 0));
    }
    else
    {
        error = errorAngle(right[i] - Point(laneWidth / 2, 0));
    }

    
    std_msgs::Float32 angle;
    std_msgs::Float32 speed;

    if(sign_name == "left"){
        angle.data = 90;
        speed.data = 30;
    }
    else if(sign_name == "right"){
        angle.data = -90;
        speed.data = 30;
    }
    else{
        angle.data = error;
        speed.data = velocity;
    }
    // angle.data = error;
    // speed.data = velocity;

    steer_publisher.publish(angle);
    speed_publisher.publish(speed);    
} 

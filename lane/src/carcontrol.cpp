#include "carcontrol.h"
// #include "sign/SignMsg.h"
CarControl::CarControl()
{
    carPos.x = 120;
    carPos.y = 300;
    steer_publisher = node_obj1.advertise<std_msgs::Float32>("Team1_steerAngle", 10);
    speed_publisher = node_obj2.advertise<std_msgs::Float32>("Team1_speed", 10);
    // sign_subcriber = node_obj3.subscribe("sign_name", 2, &CarControl::callBackSign, this);
    //  sign_subcriber = node_obj3.subscribe("sign_name", 2, &CarControl::callBackSign, this);
}

CarControl::~CarControl() {}

// void CarControl::callBackSign(const std_msgs::String::ConsttPtr &msg)
// {
//     string sign = msg->data.c_str();
//     cout << sign <<endl;
//     // if (sign == "left" or sign == "right")
//     // {
//     //     cout << sign << "-------------------------------";
//     //     hasSign = true;
//     //     BRAKE = true;
//     // }
//     // double fraction = msg.fraction;
//     // if (fraction > 0.1){
//     //     BRAKE = true;
//     // }
// }

// Cost funtion
double CarControl::cost_keep_lane(double dist_closest_front, double cost_sign, double obstack_location, double dist_to_sign)
{
    // if close to sign-> don't go straight
    double cost;
    if (hasSign)
    {
        cost = cost_sign;
    }
    else
    {
        cost = 0;
    }

    return cost;
}
double CarControl::cost_change_left(double dist_closest_leftFront, double dist_closest_leftBack, double cost_sign, double obstack_location, double cost_turn_left)
{
    double cost;
    if (violate_left) // not allow turn left
    {
        cost = cost_sign;
    }
    else
    {
        // Back, front point??
        cost = cost_turn_left;
    }

    return cost;
}
double CarControl::cost_change_right(double dist_closest_rightFront, double dist_closest_rightBack, double cost_sign, double obstack_location, double cost_turn_right)
{
    double cost;
    if (violate_right) // not allow turn left
    {
        cost = cost_sign;
    }
    else
    {
        // Back, front point??
        cost = cost_turn_right;
    }

    return cost;
}

double CarControl::cost_slow_down(double cost_break)
{
    double cost = cost_break;
    cost += 10;
    return cost;
}

float CarControl::errorAngle(const Point &dst)
{
    if (dst.x == carPos.x)
        return 0;
    if (dst.y == carPos.y)
        return (dst.x < carPos.x ? -90 : 90);
    double pi = acos(-1.0);
    double dx = dst.x - carPos.x;
    double dy = carPos.y - dst.y;
    if (dx < 0)
        return -atan(-dx / dy) * 180 / pi;
    return atan(dx / dy) * 180 / pi;
}

void CarControl::driverCar(const vector<Point> &left, const vector<Point> &right, float velocity)
{
    // Control speed
    if (BRAKE)
    {
        velocity -= 20;
    }
    else if (FORWARD)
    {
        velocity += 1;
    }
    else
    {
        velocity = velocity;
    }

    // Control angle
    // Caculate cost funtion and choose minimun cost
    double continue_lane_tc;
    double break_tc;
    double left_turn_tc;
    double right_turn_tc;

    double cost_sign = 1000;
    double cost_turn_left = 200;
    double cost_turn_right = 200;   
    double cost_break = 300;
    double min_cost = 2000;     //great number

    vector<double> costs;
    // continue_lane_tc = cost_keep_lane();
    // costs.push_back(continue_lane_tc);

    // break_tc = cost_slow_down(cost_break);
    // costs.push_back(break_tc);

    // left_turn_tc = cost_change_left();
    // costs.push_back(left_turn_tc);

    // right_turn_tc = cost_change_right();
    // costs.push_back(right_turn_tc);

    // Find the action with minimum cost
    vector<String> STATE = {"go_straight",  "slow_down", "turn_left","turn_right"};
    string state = "go_straight";
    for (int i = 0; i < costs.size(); ++i)

    {
        double cost1 = costs[i];

        if (cost1 < min_cost)
        {
            min_cost = cost1;
            state = STATE[i];
        }
    }

    // Hardset
    state = "go_straight";


    // Control with steates

    // Get usefull left,right points 
    int i = left.size() - 11;
    float error = preError;
    while (left[i] == DetectLane::null && right[i] == DetectLane::null)
    {
        i--;
        if (i < 0)
            return;
    }

    // if (state == "go_straight")
    // {
    //     if (left[i] != DetectLane::null && right[i] != DetectLane::null)
    //     {
    //         error = errorAngle((left[i] + right[i]) / 2);
    //     }

    // }
    // if (state == "left")
    // {
    //     error = errorAngle(left[i] + Point(laneWidth / 2, 0));

    // }

    // if (state == "right")
    // {
    //     error = errorAngle(right[i] - Point(laneWidth / 2, 0));

    // }


    // int i = left.size() - 11;
    // float error = preError;
    // while (left[i] == DetectLane::null && right[i] == DetectLane::null)
    // {
    //     i--;
    //     if (i < 0)
    //         return;
    // }
    if (left[i] != DetectLane::null && right[i] != DetectLane::null)
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




    // Publish message 
    std_msgs::Float32 angle;
    std_msgs::Float32 speed;

    angle.data = error;
    speed.data = velocity;

    steer_publisher.publish(angle);
    speed_publisher.publish(speed);
    cout << "( angle, speed): " << angle << " " << speed << endl;
}

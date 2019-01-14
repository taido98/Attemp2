#include "carcontrol.h"
// #include "sign/SignMsg.h"



   double cost_sign = 1000;
    double cost_turn_left = 200;
    double cost_turn_right = 210;   
    double cost_break = 300;
    double min_cost = 2000;   
    float safeDelta = 15;

CarControl::CarControl()
{
    carPos.x = 120;
    carPos.y = 300;
    steer_publisher = node_obj1.advertise<std_msgs::Float32>("Team1_steerAngle", 10);
    speed_publisher = node_obj2.advertise<std_msgs::Float32>("Team1_speed", 10);
    // sign_subcriber = node_obj3.subscribe("sign_name", 2, &CarControl::callBackSign, this);
    //  sign_subcriber = node_obj3.subscribe("sign_name", 2, &CarControl::callBackSign, this);
    cvNamedWindow("PID",1);
    // cvCreateTrackbar("LowP", "PID", &t_kP, 10000);

    // cvCreateTrackbar("LowI", "PID", &t_kI, 1000);
    // // cvCreateTrackbar("HighI", "PID", &maxPID[1], 255);

    // cvCreateTrackbar("LowD", "PID", &t_kD, 1000);
    // // cvCreateTrackbar("HighD", "PID", &maxPID[2], 255);
    // cvCreateTrackbar("State", "PID", &index, 2);
    


    
    cvCreateTrackbar("Time_Forward", "PID", &time_forward, 100);    //0->5s  /20
    cvCreateTrackbar("Time_turn", "PID", &time_turn, 100);          //0->5s /20
    cvCreateTrackbar("Turn_angle", "PID", &turnAngle, 100);          //5->50s /2
    fraction_subcriber = node_obj3.subscribe("fraction", 1000, &CarControl::CallbackFraction,this);
    sign_subcriber = node_obj4.subscribe("sign_name", 1000, &CarControl::CallbackSign,this);
    
    
}

CarControl::~CarControl() {}

void CarControl::CallbackFraction(const std_msgs::Float32::ConstPtr& msg)
{
    double fraction = msg->data;
    if (fraction > 0.2){
        cout << "Prepare to see sign"<< endl;
        BRAKE = true;
        hasSign = true;
    }
    else{
        BRAKE = false;
    }

}
void CarControl::CallbackSign(const std_msgs::String::ConstPtr& msg)
{
    sign = msg->data.c_str();
    if (sign == "left" or sign == "right")
    {
        if (sign == "left")
        {
            violate_right = true;
            cost_turn_left = 0;
            index = 1;
        }
        else
        {
            violate_left = true;
            cost_turn_right = 0;
            index =2;
            
        }
        cout << "The sign is: " <<sign<<endl;
        hasSign = true;
        SIGN.push_back(1);
        BRAKE = false;
    }
    else{
        FORWARD = true;
    }
    
}


// Cost funtion
double CarControl::cost_keep_lane(double cost_sign) //dist_closest_front, double cost_sign, double obstack_location, double dist_to_sign)
{
    // if close to sign-> don't go straight
    double cost = 0;
    for (std::vector<int>::iterator it = SIGN.begin() ; it != SIGN.end(); ++it)
    {
        if (*it== 1 || hasSign == true)
        {
            cout << "Didn't go straight"<<endl;
            cost = 9999;
            return cost;
        }
        
    }
    BRAKE = false;

    return cost;
}
double CarControl::cost_change_left(double cost_turn_left)// dx, double dist_closest_leftFront, double dist_closest_leftBack, double cost_sign, double obstack_location, double cost_turn_left)
{
    double cost;
    if (violate_left) // not allow turn left
    {
        cost = 1000;//cost_sign;
    }
    else
    {
        // Back, front point??
        cost = cost_turn_left;
    }

    return cost;
}
double CarControl::cost_change_right(double cost_turn_right)//double dx,  dist_closest_rightFront, double dist_closest_rightBack, double cost_sign, double obstack_location, double cost_turn_right)
{
    double cost;
    if (violate_right) // not allow turn left
    {
        cost = 999;//cost_sign;
    }
    else
    {
        // Back, front point??
        cost = cost_turn_right;
    }

    return cost;
}

// double CarControl::cost_slow_down(double cost_break)
// {
//     double cost = cost_break;
//     cost += 10;
//     return cost;
// }

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

void CarControl::driverCar(const vector<Point> &left, const vector<Point> &right, int &velocity)
{
    // Control speed
    if (BRAKE)
    {
        ros::Time start = ros::Time::now();
        while (ros::Time::now() - start < ros::Duration(0.001)) 
        {
        //     cout << "Braking"; 
        //    std_msgs::Float32 angle;
        //     angle.data = 0;
        //    steer_publisher.publish(angle);
           std_msgs::Float32 speed;
           speed.data = 30;
           speed_publisher.publish(speed);
        
        }
        BRAKE = false;
        cout << endl;

           
    }
    if (FORWARD)
    {
        velocity = 60;
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
  //great number

    vector<double> costs;
    continue_lane_tc = cost_keep_lane(cost_sign);
    costs.push_back(continue_lane_tc);

    left_turn_tc = cost_change_left(cost_turn_left);
    costs.push_back(left_turn_tc);

    right_turn_tc = cost_change_right(cost_turn_right);
    costs.push_back(right_turn_tc);

     // break_tc = cost_slow_down(cost_break);
    // costs.push_back(break_tc);

    // Find the action with minimum cost
    vector<string> STATE = {"go_straight", "turn_left","turn_right"};
    string state;// = STATE[index];
    // cout << "Go, left, right: ";
    // int idx; 
    // for (int i = 0; i < costs.size(); ++i)

    // {
    //     double cost1 = costs[i];
    //     cout << costs[i] << " ";
    //     if (cost1 < min_cost)
    //     {
    //         min_cost = cost1;
    //         // state = STATE[i];
    //         idx = i;
    //     }
    // }
    cout << STATE[index] << "is low cost" << endl;
    // Hardset
    state = STATE[index];


    // Control with steates

    // Get usefull left,right points 
        
     float error = preError;

    if (state == "go_straight")
    {   
        cout << "State: "<< state <<endl;
        // if (prestate != "go_straight"){
        //     cout << "State: "<< state <<endl;
        // }
        
        // Like code btc
        int i = left.size() - 11;
   
        while (left[i] == DetectLane::null && right[i] == DetectLane::null)
        {
            i--;
            if (i < 0)
                return;
            }
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

    }

    else if (state == "turn_left")
    {   
        FORWARD = true;
        cout << "State: "<< state <<endl;
        // if (prestate != "turn_left"){
        //     cout << "State: "<< state <<endl;
        // }
        
      
        int i = left.size() - 7;
        while (left[i] == DetectLane::null)
        {
            i--;
            if (i < 0) return;
        }
        // using PID
        // dist = sprt(left[i].x^2+ left[i].y^2);
        Point dst = left[i];
        kP = (double)t_kP/10000.0;
        kI = (double)t_kI/100.0;
        kD = (double)t_kD/100.0;

        double pi = acos(-1.0);
        double dx = dst.x - carPos.x +safeDelta;
        double dy = carPos.y - dst.y; 

        PID pid;
        pid.Init(kP, kI, kD);
        pid.UpdateError(-dx);
        error = pid.TotalError();
        cout << "dx: " << dx<<endl;
        // error = -atan(-dx/dy) * 180 / pi;
    }

    else if (state == "turn_right")
    {
        FORWARD = true;
        // if (prestate != "turn_right"){
        //     cout << "State: "<< state <<endl;
        // }
        cout << "State: "<< state <<endl;
       
        int i = right.size() - 7;
        while (right[i] == DetectLane::null)
        {
            i--;
            if (i < 0) return;
        }
        // using PID
        // dist = sprt(left[i].x^2+ left[i].y^2);
        Point dst = right[i];
        kP = (double)t_kP/10000.0;
        kI = (double)t_kI/100.0;
        kD = (double)t_kD/100.0;

        double pi = acos(-1.0);
        double dx = dst.x - carPos.x -safeDelta;
        double dy = carPos.y - dst.y; 
        
        PID pid;
        pid.Init(kP, kI, kD);
        pid.UpdateError(-dx);
        error= pid.TotalError();
        cout << "dx: " << dx<<endl;
        // error = dx;
    }

    prestate = state;
    // int i = left.size() - 11;
    // float error = preError;
    




    // Publish message 
   
    std_msgs::Float32 angle;
    std_msgs::Float32 speed;
    angle.data = error;
    speed.data = velocity;

    steer_publisher.publish(angle);
    speed_publisher.publish(speed);
    // cout << "( angle, speed): " << angle << " " << speed << endl;
}




void CarControl::driverCar2(const vector<Point> &left, const vector<Point> &right, int &velocity)
{
    // Hardset for turn left 
    if (sign == "left")

    {
        ROS_INFO_STREAM("TURN LEFT SIGN DETECTED");
        // Reduce speed and go for 2 meters
        ros::Time start = ros::Time::now();
        while (ros::Time::now() - start < ros::Duration(time_forward/20))
         {
            std_msgs::Float32 angle;
            angle.data = 0;
           steer_publisher.publish(angle);
           std_msgs::Float32 speed;
           speed.data = 30;
           speed_publisher.publish(speed);        
        }
        // Stop for 3 seconds before moving
        start = ros::Time::now();
        while (ros::Time::now() - start < ros::Duration(time_turn/20)) {
            std_msgs::Float32 angle;
            angle.data = -turnAngle/2;
           steer_publisher.publish(angle);
           std_msgs::Float32 speed;
           speed.data = 30;
           speed_publisher.publish(speed);        }

        
        
        sign = "none";
        // area = 0;
        // flag = false;
        
    }
     // Hardset for turn left 
    if (sign == "right")

    {
        ROS_INFO_STREAM("TURN RIGHT SIGN DETECTED");
        // Reduce speed and go for 2 meters
        ros::Time start = ros::Time::now();
        while (ros::Time::now() - start < ros::Duration(time_forward/20)) 
        {
            std_msgs::Float32 angle;
            angle.data = 0;
           steer_publisher.publish(angle);
           std_msgs::Float32 speed;
           speed.data = 20;
           speed_publisher.publish(speed);        
        }
        // Stop for 3 seconds before moving
        start = ros::Time::now();
        while (ros::Time::now() - start < ros::Duration(time_turn/20))
         {
            std_msgs::Float32 angle;
            angle.data = turnAngle/2;
           steer_publisher.publish(angle);
           std_msgs::Float32 speed;
           speed.data = 20;
           speed_publisher.publish(speed);        
        }
        
        sign = "none";
        // area = 0;
        // flag = false;
        
    }
    else{
        // cout << "Drive nornally"<<endl;
        float error = preError; 

        int i = left.size() - 11;
   
        while (left[i] == DetectLane::null && right[i] == DetectLane::null)
        {
            i--;
            if (i < 0)
                return;
        }
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

        
        std_msgs::Float32 angle;
        std_msgs::Float32 speed;
        angle.data = error;
        speed.data = velocity;

        steer_publisher.publish(angle);
        speed_publisher.publish(speed);
    }

}






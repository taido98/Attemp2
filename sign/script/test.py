#! usr/bin/env python
from time import time
import numpy as np
import cv2
import imutils
# import matplotlib.pyplot as plt

import rospy
import math
from sensor_msgs.msg import CompressedImage
from std_msgs.msg import Float32


from land_detect import lane_detection as LD
from land_detect.Line import Line
from car_control.PID import PID
from traffic_detect import traffic_detect as TD
from car_control.carcontrol import CarControl

def Detect(image):
    HEIGHT = image.shape[0]
    final_img, left_line, right_line = LD.color_frame_pipeline([image], solid_lines=True)
    x_axis, y_axis = right_line.seperate_axis()
    pol_right = np.polyfit(x_axis,y_axis,1)
    rm  = pol_right[0]
    rb  = pol_right[1]
    x_axis, y_axis = left_line.seperate_axis()
    pol_left = np.polyfit(x_axis,y_axis,1)
    lm  = pol_left[0]
    lb  = pol_left[1]
    yb = HEIGHT 
    yt = yb/2
    xt_r = (yt - rb) / rm
    xb_r = (yb - rb) / rm
    xt_l = (yt - lb) / lm
    xb_l = (yb - lb) / lm   

    left_line = Line(xb_l,yb,xt_l,yt)
    right_line = Line(xb_r,yb,xt_r,yt)

    control = CarControl(direction = 'left',cte = 0,speed = 60,sign = None,brake = False, right_line = None, left_line = None)
    
    print(control.angle,control.speed,"(angle,speed)")

    return final_img

def main():
    rospy.init_node('test')
    s = time()
    img = cv2.imread('Image/right4.png')
    # img = cv2.imread('Image/left.png')
    final_img = Detect(img)
    cv2.imshow('image', final_img)
    e = time()
    print(e-s,'duration')
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    

if __name__ == '__main__':
    main()
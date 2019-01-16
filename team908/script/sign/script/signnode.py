#!/usr/bin/env python
import sys,os

from time import time
import numpy as np
import cv2

import rospy
import math
from sensor_msgs.msg import CompressedImage
from std_msgs.msg import Float32,String



# from carcontrol import CarControl
from detectsign import DetectSign

class Sign:
    def __init__(self):
        '''Initialize ros publisher, ros subscriber'''
        self.subscriber = rospy.Subscriber("Team1_image/compressed",

                                    CompressedImage, self.callback)
        self.pubsign = rospy.Publisher("sign_name",String, queue_size=2)
        self.pubfrac = rospy.Publisher("fraction",Float32, queue_size=2)
        # self.sign = rospy.Publisher("lane_detect", String, queue_size = 10)
        self.sign = None
    def callback(self, ros_data):
        '''Callback function of subscribed topic.  Here images get converted and features detected'''
        np_arr = np.fromstring(ros_data.data, np.uint8)
        # out = cv2.VideoWriter('../videos/output.avi'.format(np.random.randint(1,50)),fourcc, 20.0, (640,480))
        image_np = cv2.imdecode(np_arr, cv2.IMREAD_COLOR)#cv2.CV_LOAD_IMAGE_COLOR)

        
        # fourcc = cv2.VideoWriter_fourcc(*'XVID')
    
        
        # out.write(image_np)
        Sign = DetectSign(image_np)
        start = rospy.Time.now()
        if (Sign.sign is not None):
            while (rospy.Time.now() - start < rospy.Duration(0.5)):
                self.pubsign.publish(Sign.sign)
                self.pubfrac.publish(Sign.fraction_sign)
        # self.pubsign.publish(Sign.sign)
        # self.pubfrac.publish(Sign.fraction_sign)
        cv2.imshow('image',Sign.image)
        k = cv2.waitKey(1)
        if k == ord('q'):         # wait for ESC key to exit
            cv2.destroyAllWindows()
            rospy.signal_shutdown('Exit')
def main(args):
    rospy.init_node('sign')
    ic = Sign()
    
    try:
        rospy.spin()    
            
    except KeyboardInterrupt:
        print ("Shutting down ROS Image feature detector module")
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main(sys.argv)
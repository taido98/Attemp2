#! usr/bin/env python
import sys
sys.path.append('../lane')
sys.path.append('../sign')
sys.path.append('../lane')
sys.path.append('..')
from time import time
import numpy as np
import cv2
import imutils
# import lane_detection as LD
# from Line import Line
import traffic_detect as TD
import argparse

import rospy
import math
from sensor_msgs.msg import CompressedImage
from std_msgs.msg import Float32,String

# from pyimagesearch.preprocessing.imagetoarraypreprocessor import ImageToArrayPreprocessor
# from pyimagesearch.preprocessing.simplepreprocess import SimplePreprocessor
# from pyimagesearch.datasets.simpledatasetloader import SimpleDatasetLoader
# from pyimagesearch.nn.conv.shallownet import ShallowNet
# from keras.models import load_model
# from imutils import paths
# import numpy as np


def constrastLimit(image):
        img_hist_equalized = cv2.cvtColor(image, cv2.COLOR_BGR2YCrCb)
        channels = cv2.split(img_hist_equalized)
        channels[0] = cv2.equalizeHist(channels[0])
        img_hist_equalized = cv2.merge(channels)
        img_hist_equalized = cv2.cvtColor(img_hist_equalized, cv2.COLOR_YCrCb2BGR)
        return img_hist_equalized

def LaplacianOfGaussian(image):
    LoG_image = cv2.GaussianBlur(image, (3,3), 0)           # paramter 
    gray = cv2.cvtColor( LoG_image, cv2.COLOR_BGR2GRAY)
    LoG_image = cv2.Laplacian( gray, cv2.CV_8U,3,3,2)       # parameter
    LoG_image = cv2.convertScaleAbs(LoG_image)
    return LoG_image
def region_of_interest(img, vertices):
    """
    Applies an image mask.

    Only keeps the region of the image defined by the polygon
    formed from `vertices`. The rest of the image is set to black.
    """

    # defining a blank mask to start with
    mask = np.zeros_like(img)

    cv2.fillPoly(mask, vertices, 255)

    # returning the image only where mask pixels are nonzero
    masked_image = cv2.bitwise_and(img, mask)

    return masked_image, mask
    
def binarization(image):
    thresh = cv2.threshold(image,32,255,cv2.THRESH_BINARY)[1]
    #thresh = cv2.adaptiveThreshold(image,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY,11,2)
    return thresh

class DetectSign:
    def __init__(self,img):
        self.image = img
        self.box = None
        self.fraction_obatack = 0
        self.fraction_sign = 0
        self.presign = None 
        self.sign = None
        self.masked = None
        self.mask_dark = None
        # self.vertices = np.array([[(int(0*img.shape[1]),img.shape[0]),(int(0*img.shape[1]), 
        #             int(0.15*img.shape[0])), (int(1.00*img.shape[1]), int(0.15*img.shape[0])), (
        #             img.shape[1],img.shape[0])]], dtype=np.int32)
        self.vertices = np.array([[(0,240),(50,60) , (260,50),(320,240)
                ]], dtype=np.int32)
        self.find_fraction_sign()
        if (self.fraction_sign > 3):
            self.find_sign()

        self.find_fraction_obstack()
        if (self.fraction_obstack > 2.7):  #2.5
            self.find_obstack()
        # # full_param_name = rospy.search_param('sign')
        # # # # param_value = rospy.get_param(full_param_name)
        # # # print(type(full_param_name))
        # # param_value = rospy.get_param(full_param_name)
        # # print((param_value))

    def find_fraction_sign(self):
        x = np.random.randint(10,180)
        y = np.random.randint(10,180)
        z = np.random.randint(30,100)
        other = self.image[x:x+z,y:y+z]
        # cv2.imwrite('../data/none/{}.png'.format(np.random.randint(1,10000)),other)
        lower_blue = np.array([90,110,50])
        upper_blue = np.array([110,255,255])
        hsv = cv2.cvtColor(self.image, cv2.COLOR_BGR2HSV)

        kernel = np.ones((3,3),np.uint8)
        mask = cv2.inRange(hsv, lower_blue, upper_blue)
        mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)
        mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)
        self.masked = mask
        topBlock = mask[0:100, 0:320]
        fraction = np.sum(topBlock)/(320*100)
        self.fraction_sign =fraction
        # print(fraction,"fracrion")
    def find_sign(self):
        try:
        # start = time()
            sign,self.box= TD.detect_traffic_sign(self.image,self.masked)       ##sign_y is bottom vertical position of sign
            # sign,self.box = TD.detect_traffic_sign_NN(self.image,self.masked)
            # end = time()
            
            # print(end-start,"duration detect sign")
            if (self.box[1][1]) > 30:
                self.sign = sign
                

            self.presign = self.sign
            cv2.putText(self.image, self.sign, (self.box[1][0]-55,self.box[1][1]+15), cv2.FONT_HERSHEY_SIMPLEX, 0.65, (0, 255, 0), 2) 
            # cv2.imshow('image',self.image)
        except:
            pass
            # print("Error")
    def find_fraction_obstack(self):
        frame = cv2.GaussianBlur(self.image, (3,3), 0) 
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        # define range of blue color in HSV
        lower_dark = np.array([0,0,0])
        upper_dark = np.array([170,150,50])
        # Threshold the HSV image to get only blue colors
        mask_dark = cv2.inRange(hsv, lower_dark, upper_dark)
        kernel = np.ones((3,3),np.uint8)
        mask = cv2.morphologyEx(mask_dark, cv2.MORPH_OPEN, kernel)
        mask = cv2.morphologyEx(mask_dark, cv2.MORPH_CLOSE, kernel)
        # mask_dark = cv2.cvtColor(mask_dark, cv2.COLOR_HSV2RGB)
        self.mask_dark = mask
        topBlock = mask[0:150, 50:270]
        fraction = np.sum(topBlock)/(150*220)
        self.fraction_obstack = fraction
        #print(fraction ,"obstack: ")
    

    def preprocess_image(self):
        image = constrastLimit(self.image)
        image = LaplacianOfGaussian(image)
        image = binarization(image)
        image = cv2.bitwise_and(image,self.mask_dark)
        return image

    def find_obstack(self):
        binary_image = self.preprocess_image()
        mased_image, mask = region_of_interest(binary_image,self.vertices)
        # cv2.imshow("mask",mased_image)
        
        _, cnts, hierarchy = cv2.findContours(mased_image,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
        bottom = []
        left = []
        right = []
        top = []
        for i,cnt in enumerate(cnts,0):
        #     print(len(cnt))
            coordinate = np.reshape(cnt, [-1,2])
        #     print(coordinate.shape)
        #     print(np.amin(coordinate, axis=0)[0])
            b,l= np.amin(coordinate, axis=0)
            if (l >40):
                bottom.append(b)
                left.append(l)
            r, t = np.amax(coordinate, axis = 0).astype(int)
            if (r< 280):
                right.append(r)
                top.append(t)
        bottom= np.amin(bottom, axis=0)
        left = np.amin(left,axis=0)
        top = np.amax(top,axis=0)
        right = np.amax(right,axis=0)
        # print(bottom,left,top,right)


        # rect = bottom,left,top,right;
        # # box = cv2.boxPoints(rect) 
        # z = self.image[left-5:top+5,bottom-5:right+5]    
        # cv2.imshow("ob",z)           # wait for ESC key to exit
        # # cv2.imwrite('ss/{}.png'.format(np.random.randint(1,20)),self.image[ left:right, top:bottom] )
        # # z = self.image[ top:bottom,left:right ]               # wait for ESC key to exit
        # cv2.imwrite('ss/{}.png'.format(np.random.randint(0,100)),z)

        cv2.rectangle(self.image,(bottom-2,left-2), (right+1,top+1),(0,0,255),2)



# def main():
#     ap = argparse.ArgumentParser()
#     ap.add_argument("-i","--image",required=True)
#     args = vars(ap.parse_args())

#     image = cv2.imread(args["image"])
#     Sign = DetectSign(image)
#     cv2.imshow('image',Sign.image)
#     cv2.waitKey(0)
#     # print(Sign.sign)
# if __name__ == '__main__':
#     main()


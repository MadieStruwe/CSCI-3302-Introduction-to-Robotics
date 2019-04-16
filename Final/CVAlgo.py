import cv2
from numpy import *
from pylab import *
#from imgops import imutils
import math

def invert_img(img):
    img = (255-img)
    return img


def canny(imgray):
    imgray = cv2.GaussianBlur(imgray, (11,11), 200)
    canny_low = 0
    canny_high = 100

    thresh = cv2.Canny(imgray,canny_low,canny_high)
    return thresh

def cnt_gui(img, contours):
    cnts = sorted(contours, key = cv2.contourArea, reverse = True)

    for i in range(0,len(cnts)):
        sel_cnts = sorted(contours, key = cv2.contourArea, reverse = True)[i]

        area = cv2.contourArea(sel_cnts)

        if area < 1000:
            continue

        # get orientation angle and center coord
        center, axis,angle = cv2.fitEllipse(sel_cnts)

        hyp = 100  # length of the orientation line

        # Find out coordinates of 2nd point if given length of line and center coord 
        linex = int(center[0]) + int(math.sin(math.radians(angle))*hyp)
        liney = int(center[1]) - int(math.cos(math.radians(angle))*hyp)

        # Draw orienation
        cv2.line(img, (int(center[0]),int(center[1])), (linex, liney), (0,0,255),5)             
        cv2.circle(img, (int(center[0]), int(center[1])), 10, (255,0,0), -1)

    return img

def filtering(img, imgray, mode):
    imgray = cv2.medianBlur(imgray, 11)
    thresh = cv2.Canny(imgray,75,200)

    return thresh, imgray
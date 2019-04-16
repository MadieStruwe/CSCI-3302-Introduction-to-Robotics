#python cap_video.py

import cv2
import numpy as np

im = cv2.imread('vs.jpg')
imgray = cv2.cvtColor(im,cv2.COLOR_BGR2GRAY)
ret,thresh = cv2.threshold(imgray,127,255,0)
contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)

img = cv2.drawContours(im, contours, -1, (0,255,0), 3)

cv2.imshow('???', im)
 
cv2.waitKey(0)
# Closes all the frames
cv2.destroyAllWindows()
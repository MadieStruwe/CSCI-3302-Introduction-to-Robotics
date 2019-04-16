import cv2
import numpy as np
from datetime import datetime
  
webcam = cv2.VideoCapture(0)
 
while True:
     
    # get image from webcam
    image = webcam.read()
 
    # display image
    cv2.imshow('grid', image)
    cv2.waitKey(3000)
 
    # save image to file, if pattern found
    ret, corners = cv2.findChessboardCorners(cv2.cvtColor(image,cv2.COLOR_BGR2GRAY), (7,6), None)
 
    if ret == True:
        filename = datetime.now().strftime('%Y%m%d_%Hh%Mm%Ss%f') + '.jpg'
        cv2.imwrite("pose/sample_images/" + filename, image)
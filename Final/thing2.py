#python cap_video.py

import cv2
import numpy as np
import math

# Create a VideoCapture object and read from input file
# If the input is the camera, pass 0 instead of the video file name
cap = cv2.VideoCapture(0)
 
# Check if camera opened successfully
if (cap.isOpened()== False): 
  print("Cannot Open Camera")
 
# Read until video is completed
while(cap.isOpened()):
  # Capture frame-by-frame
  ret, frame = cap.read()
  if ret == True:
    # Display the resulting frame
    #cv2.imshow('Frame',frame)
    #make it gray
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    #show the gray bby
    #cv2.imshow ("gray", gray)
    #set the threshold for contouring
    ret2, thresh = cv2.threshold(gray, 127, 255, 0)
    #show those thresholds bby
    #cv2.imshow ("thresh", thresh)
    #heres where we find the coutours
    #contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    contours,hierarchy = cv2.findContours(thresh, 1, 2)
    img = cv2.drawContours(frame, contours, -1, (0,255,0), 3)
    #this will display the contrours on the video
    cv2.imshow('contrours on video', frame)
    cnts=sorted(contours,key=cv2.contourArea, reverse=True)
    for i in range(0,len(cnts)):
    	sel_cnts=sorted(contours, key=cv2.contourArea,reverse=True)[i]
    	area = cv2.contourArea(sel_cnts)
    	center, axis,angle=cv2.fitEllipse(sel_cnts)
    	hyp=100 # length of the orientation line

        # Find out coordinates of 2nd point if given length of line and center coord 
        linex = int(center[0]) + int(math.sin(math.radians(angle))*hyp)
        liney = int(center[1]) - int(math.cos(math.radians(angle))*hyp)

        # Draw orienation
        cv2.line(frame, (int(center[0]),int(center[1])), (linex, liney), (0,0,255),5)             
        cv2.circle(frame, (int(center[0]), int(center[1])), 10, (255,0,0), -1)

        cv2.imshow('kill me',frame)
 
    # Press Q on keyboard to  exit
    if cv2.waitKey(25) & 0xFF == ord('q'):
      break
 
# When everything done, release the video capture object
cap.release()
 
# Closes all the frames
cv2.destroyAllWindows()
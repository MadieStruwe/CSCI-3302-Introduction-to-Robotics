#code source https://www.pyimagesearch.com/2014/12/15/real-time-barcode-detection-video-python-opencv/

import cv2
import numpy as np
import simple_barcode_detection

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
  	cv2.imshow('Frame',frame)
  	#make the grayssss
  	#gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
  	#show video of the gray capture
  	#cv2.imshow('Gray', gray)
  	# detect the barcode in the image
  	box = simple_barcode_detection.detect(frame)
  	# if a barcode was found, draw a bounding box on the frame
  	cv2.drawContours(frame, [box], -1, (0, 255, 0), 2)
  	# show the frame and record if the user presses a key
  	cv2.imshow("Frame", frame)
  	key = cv2.waitKey(1) & 0xFF
  	# if the 'q' key is pressed, stop the loop
  	if key == ord("q"):
  		break

# cleanup the camera and close any open windows
cap.release()
cv2.destroyAllWindows()
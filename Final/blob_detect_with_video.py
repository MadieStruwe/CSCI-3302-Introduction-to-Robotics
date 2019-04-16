#python blob_detect_with_video.py

# Standard imports
import cv2
import argparse
import imutils
import numpy as np


# Create a VideoCapture object and read from input file
# If the input is the camera, pass 0 instead of the video file name
cap = cv2.VideoCapture(0)

# Set up the detector with parameters.
params = cv2.SimpleBlobDetector_Params()
 
# Check if camera opened successfully
if (cap.isOpened()== False): 
  print("Cannot Open Camera")
 
# Read until video is completed
while(cap.isOpened()):
  # Capture frame-by-frame
  ret, frame = cap.read()
  if ret == True:

  	# Press Q on keyboard to  exit
	if cv2.waitKey(25) & 0xFF == ord('q'):
	  break

	# grab the current frame
	(grabbed, frame) = cap.read()

	# resize the frame
	frame = imutils.resize(frame, width=600) 

	# Convert BGR to RGB
	rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

	params.filterByColor= True
	params.blobColor= 255
	
	# Create a detector with the parameters
	ver = (cv2.__version__).split('.')
	if int(ver[0]) < 3 :
		detector = cv2.SimpleBlobDetector(params)
	else : 
		detector = cv2.SimpleBlobDetector_create(params)
	 
	# Detect blobs.
	keypoints = detector.detect(frame)
	 
	# Draw detected blobs as red circles.
	# cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS ensures the size of the circle corresponds to the size of blob
	image_with_keypoints = cv2.drawKeypoints(frame, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
	 
	# Show keypoints
	cv2.imshow("Keypoints", image_with_keypoints)

	k = cv2.waitKey(1) & 0xFF
	if k == 27:
		qbreak

	

# When everything done, release the video capture object
cap.release()
 
# Closes all the frames
cv2.destroyAllWindows()





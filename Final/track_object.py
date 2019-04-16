from collections import deque
import numpy as np
import argparse
import imutils
import cv2

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
 
    # Press Q on keyboard to  exit
    if cv2.waitKey(1) & 0xFF == ord('q'):
      break

    # grab the current frame
    (grabbed, frame) = cap.read()
 
    # resize the frame
    frame = imutils.resize(frame, width=600) 

    # Convert BGR to RGB
    rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

    # define range of colors in RGB
    lower_blue = np.array([15,20,26])
    upper_blue = np.array([38,58,119])
    blue_mask = cv2.inRange(rgb, lower_blue, upper_blue)

    lower_green = np.array([76,52,16])
    upper_green = np.array([131,105,68]) 
    green_mask = cv2.inRange(rgb, lower_green, upper_green)

    lower_red = np.array([36,2,3])
    upper_red = np.array([113,38,45])
    red_mask = cv2.inRange(rgb, lower_red, upper_red)

    # mask to get multi colors at once
    mask = blue_mask + green_mask + red_mask
    mask_red=red_mask
    mask_blue=blue_mask

    # construct a mask for the color "red", then perform
    # a series of dilations and erosions to remove any small
    # blobs left in the mask
    mask_red = cv2.inRange(rgb, lower_red, upper_red)
    mask_red = cv2.erode(mask_red, None, iterations=2)
    mask_red = cv2.dilate(mask_red, None, iterations=2)

    # construct a mask for the color "blue", then perform
    # a series of dilations and erosions to remove any small
    # blobs left in the mask
    mask_blue = cv2.inRange(rgb, lower_blue, upper_blue)
    mask_blue = cv2.erode(mask_blue, None, iterations=2)
    mask_blue = cv2.dilate(mask_blue, None, iterations=2)

    # find contours in the mask and initialize the current
    # (x, y) center of the ball
    cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
        cv2.CHAIN_APPROX_SIMPLE)[-2]
    center = None
 
    # only proceed if at least one contour was found
    if len(cnts) > 0:
        # find the largest contour in the mask, then use
        # it to compute the minimum enclosing circle and
        # centroid
        c = max(cnts, key=cv2.contourArea)
        ((x, y), radius) = cv2.minEnclosingCircle(c)
        M = cv2.moments(c)
        center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))
 
        # only proceed if the radius meets a minimum size
        if radius > 10:
            # draw the circle and centroid on the frame,
            # then update the list of tracked points
            cv2.circle(frame, (int(x), int(y)), int(radius),
                (0, 255, 255), 2)
 

    # Bitwise-AND mask and original image
    res = cv2.bitwise_and(frame,frame, mask= mask)
    res_red = cv2.bitwise_and(frame,frame, mask= mask_red)
    res_blue = cv2.bitwise_and(frame,frame, mask= mask_blue)

    #blur (median)
    blur = cv2.medianBlur(res,5)
    blur_red=cv2.medianBlur(res_red,5)
    blur_blue=cv2.medianBlur(res_blue,5)

    #display results
    cv2.imshow('Frame',frame)
    cv2.imshow('Red Mask',res_red)
    cv2.imshow('Blue Mask', res_blue)
    cv2.imshow('Color Mask',res)
    k = cv2.waitKey(1) & 0xFF
    if k == 27:
        break

cv2.destroyAllWindows()
import cv2
import numpy as np

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

    # Convert BGR to RGB
    rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

    # define range of blue color in RGB
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

    # Bitwise-AND mask and original image
    res = cv2.bitwise_and(frame,frame, mask= mask)


    #Blur video to reduce noise
    #blur (normal)
    ##blur=cv2.blur(res,(5,5))
    #blur (gaussian)
    ##blur = cv2.GaussianBlur(res,(5,5),0)
    #blur (median)
    blur = cv2.medianBlur(res,5)

    #display results
    cv2.imshow('Frame',frame)
    cv2.imshow('Mask',mask)
    cv2.imshow('Color Mask',blur)
    k = cv2.waitKey(1) & 0xFF
    if k == 27:
        break

cv2.destroyAllWindows()
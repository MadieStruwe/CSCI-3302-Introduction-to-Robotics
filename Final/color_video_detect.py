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

    # Take each frame
    _, frame = cap.read()

    # Convert BGR to HSV
    rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

    # define range of blue color in RGB
    lower_blue = np.array([15,32,62])
    upper_blue = np.array([116,140,180])
    blue_mask = cv2.inRange(rgb, lower_blue, upper_blue)

    lower_green = np.array([50, 50, 120])
    upper_green = np.array([70, 255, 255]) 
    green_mask = cv2.inRange(rgb, lower_green, upper_green)

    lower_red = np.array([141,40,56])
    upper_red = np.array([197,72,80])
    red_mask = cv2.inRange(rgb, lower_red, upper_red)

    # mask to get multi colors at once
    mask = blue_mask + green_mask + red_mask

    # Bitwise-AND mask and original image
    res = cv2.bitwise_and(frame,frame, mask= mask)

    #display results
    cv2.imshow('Frame',frame)
    cv2.imshow('Mask',mask)
    cv2.imshow('Color Mask',res)
    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break

cv2.destroyAllWindows()
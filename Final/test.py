#python test.py
import cv2
import numpy as np


# Create a VideoCapture object and read from input file
# If the input is the camera, pass 0 instead of the video file name
cap = cv2.VideoCapture(0)
 
# Check if camera opened successfully
if (cap.isOpened()== False): 
  print("Cannot Open Camera")

#camara size
cap.set(3,800) #width
cap.set(4,600) #height
 
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

    # Convert BGR to RGB
    rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

    # define range of blue color in RGB
    lower_blue = np.array([2,25,77])
    upper_blue = np.array([48,96,207])
    blue_mask = cv2.inRange(rgb, lower_blue, upper_blue)

    lower_green = np.array([1,18,2])
    upper_green = np.array([78,84,36]) 
    green_mask = cv2.inRange(rgb, lower_green, upper_green)

    lower_red = np.array([41,6,10])
    upper_red = np.array([90,30,32])
    red_mask = cv2.inRange(rgb, lower_red, upper_red)

    # mask to get multi colors at once
    mask = blue_mask + green_mask + red_mask

    # Bitwise-AND mask and original image
    res = cv2.bitwise_and(frame,frame, mask= mask)

    #blur the frames
    #normal blur
    ##blur = cv2.blur(res,(5,5))
    #gaussian blur
    ##blur = cv2.GaussianBlur(res,(5,5),0)
    #median blur
    blur = cv2.medianBlur(res,5)

    # Set up the detector with parameters.
    #params = cv2.SimpleBlobDetector_Params()
    #detector = cv2.SimpleBlobDetector()
    params = cv2.SimpleBlobDetector_Params()

    # Change thresholds
    params.minThreshold = 0;
    params.maxThreshold = 256;

    #Filter by Area
    params.filterByArea = True
    params.minArea = 250 #blobs of pixels

    # Filter by Circularity
    params.filterByCircularity = True
    params.minCircularity = 0.001

    # Filter by Convexity
    params.filterByConvexity = True
    params.minConvexity = 0.87
    
    # Filter by Inertia
    #params.filterByInertia = True
    #params.minInertiaRatio = 0.01

    #Filter by Color
    # 0 for dark blobs, 255 for light blobs
    #params.filterByColor= True
    #params.blobColor= 255

    # Create a detector with the parameters
    ver = (cv2.__version__).split('.')
    if int(ver[0]) < 3 :
        detector = cv2.SimpleBlobDetector(params)
    else : 
        detector = cv2.SimpleBlobDetector_create(params)
     
    # Detect blobs.
    keypoints = detector.detect(blur)

    #trying to get coordinates of blobs
    for keyPoint in keypoints:
        x=keyPoint.pt[0]
        y=keyPoint.pt[1]
        #s=keyPoint.size
        print (x, y)
     
    # Draw detected blobs as red circles.
    # cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS ensures the size of the circle corresponds to the size of blob
    image_with_keypoints = cv2.drawKeypoints(blur, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

    revmask=255-blur
    key=detector.detect(revmask)
    image_key=cv2.drawKeypoints(blur, key, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

     
    # Show keypoints
    #display results
    #cv2.imshow("Color Mask and Blob Detection", image_with_keypoints)
    cv2.imshow("Color Mask and Blob Detection BETTER??", image_key)
    cv2.imshow('Frame',frame)
    #cv2.imshow('Mask',mask)
    #cv2.imshow('Color Mask',res)
    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break

cv2.destroyAllWindows()

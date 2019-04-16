#python detect_color_and_blur.py --image <name.ex>

# import the necessary packages
import numpy as np
import argparse
import cv2
from matplotlib import pyplot as plt
 
# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", help = "path to the image")
args = vars(ap.parse_args())
 
# load the image
image = cv2.imread(args["image"])

b,g,r=cv2.split(image)
image_adj=cv2.merge([r,g,b])

#lower then upper, in bgr
# define the list of boundaries
boundaries = [
	#brent.jpg
	##([24,23,39],[138,59,224])
	#duck.jpg
	([127,132,147],[254,254,254])
	#vs.jpg
	##([9,6,28],[40,80,239]),
	##([254,240,234],[254,254,254])
]

# loop over the boundaries
for (lower, upper) in boundaries:
	# create NumPy arrays from the boundaries
	lower = np.array(lower, dtype = "uint8")
	upper = np.array(upper, dtype = "uint8")
 
	# find the colors within the specified boundaries and apply
	# the mask
	mask = cv2.inRange(image, lower, upper)
	output = cv2.bitwise_and(image, image, mask = mask)
 
# show the images
cv2.imshow("images", np.hstack([image, output]))
cv2.waitKey(0)

#normal blur
##blur = cv2.blur(img,(5,5))
#gaussian blur
##blur = cv2.GaussianBlur(img,(5,5),0)
#median blur
blur = cv2.medianBlur(output,5)

b,g,r=cv2.split(output)
blur_adj=cv2.merge([r,g,b])
 
plt.subplot(121),plt.imshow(image_adj),plt.title('Original')
plt.xticks([]), plt.yticks([])
plt.subplot(122),plt.imshow(blur_adj),plt.title('Blurred')
plt.xticks([]), plt.yticks([])
plt.show()
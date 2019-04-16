#python blur.py
import cv2
import numpy as np
from matplotlib import pyplot as plt

img = cv2.imread('brent.jpg')

#normal blur
##blur = cv2.blur(img,(5,5))
#gaussian blur
##blur = cv2.GaussianBlur(img,(5,5),0)
#median blur
blur = cv2.medianBlur(img,5)

b,g,r=cv2.split(img)
blur_adj=cv2.merge([r,g,b])
 
plt.subplot(121),plt.imshow(img),plt.title('Original')
plt.xticks([]), plt.yticks([])
plt.subplot(122),plt.imshow(blur_adj),plt.title('Blurred')
plt.xticks([]), plt.yticks([])
plt.show()
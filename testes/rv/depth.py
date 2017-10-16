import numpy as np
import cv2
from matplotlib import pyplot as plt

# imgR = cv2.imread('/home/opcinove/Downloads/rgbd_dataset_freiburg1_xyz/rgb/1305031102.175304.png',0)
# imgL = cv2.imread('/home/opcinove/Downloads/rgbd_dataset_freiburg1_xyz/rgb/1305031102.211214.png',0)

imgR = cv2.imread('rgb/000001.png',0)
imgL = cv2.imread('rgb/000002.png',0)

stereo = cv2.StereoBM_create(numDisparities=32, blockSize=19)
print ("jsfgsja")
disparity = stereo.compute(imgL,imgR)
print ("teste")
plt.imshow(disparity,'gray')
plt.show()
# plt.imshow(imgL)
import cv2
import numpy as np
from TrafficSignDetection import *

cap=cv2.VideoCapture(0)
while(True):
	_, frame = cap.read()
	img = returnRedness(frame)
	img = threshold(img,T = 150)
	redness = returnRedness(frame) # step 1 --> specify the redness of the image
	thresh = threshold(redness) 	
	try:
		contours = findContour(thresh)
		big = findBiggestContour(contours)
		if cv2.contourArea(big) > 3000:
			print(cv2.contourArea(big))
			img,sign = boundaryBox(frame,big)
			cv2.imshow('frame',img)
			cv2.imshow('sign',sign)
		else:
			cv2.imshow('frame',frame)
	except:
		cv2.imshow('frame',frame)
		
	
cv2.waitKey(0)
cv2.destroyAllWindows()
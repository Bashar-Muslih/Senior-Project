# Write your code here :-)
import cv2
from picamera import PiCamera

frameWidth = 640
frameHeight= 480
cap = cv2.VideoCapture(-1)
cap.set(3,frameWidth)
cap.set(4,frameHeight)
while True:
    sucess, img =cap.read()
    cv2.imshow("Results", img)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
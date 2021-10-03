from ObjectDetectorModule import *

cap = cv2.VideoCapture(-1)
cap.set(3,640)
cap.set(4,480)
#cap.set(10,70)
while True:
    success,img = cap.read()
    result,objectInfo = getObjects(img, 0.45, 0.2, objects=['cup', 'bottle']) #chooses which one to detect
    print(objectInfo)
    cv2.imshow("Output",img)
    cv2.waitKey(1)
# top left origin, bottom right 640,480
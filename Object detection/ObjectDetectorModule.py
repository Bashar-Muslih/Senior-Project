import cv2
import serial
import time

thres = 0.45 # Threshold to detect object

classNames= []
classFile = '/home/pi/Downloads/Objectcv/coco.names'
with open(classFile,'rt') as f:
    classNames = f.read().rstrip('\n').split('\n')

configPath = '/home/pi/Downloads/Objectcv/ssd_mobilenet_v3_large_coco_2020_01_14.pbtxt'
weightsPath = '/home/pi/Downloads/Objectcv/frozen_inference_graph.pb'

net = cv2.dnn_DetectionModel(weightsPath,configPath)
net.setInputSize(320,320)
net.setInputScale(1.0/ 127.5)
net.setInputMean((127.5, 127.5, 127.5))
net.setInputSwapRB(True)

def getObjects(img,thres,nms,draw = True,objects=[]):
    classIds, confs, bbox = net.detect(img,confThreshold=thres,nmsThreshold=nms) #to avoid duplication of detection
    #print(classIds,bbox)
    if len(objects) ==0: objects = classNames #allows to choose which object to detect and display
    objectInfo = []
    if len(classIds) != 0:
        for classId, confidence,box in zip(classIds.flatten(),confs.flatten(),bbox):
            className = classNames[classId-1]
            if className in objects: # if the classname is in the object then permorm this
                objectInfo.append([box, className])
                if (draw):
                    cv2.rectangle(img,box,color=(0,255,0),thickness=2)
                    cv2.putText(img,className.upper(),(box[0]+10,box[1]+30),
                            cv2.FONT_HERSHEY_COMPLEX,1,(0,255,0),2)
                    cv2.putText(img,str(round(confidence*100,2)),(box[0]+200,box[1]+30),
                            cv2.FONT_HERSHEY_COMPLEX,1,(0,255,0),2)
                    #print("box[0] = ", box[0], "| box[1] = ", box[1]," | box[2] = ", box[2]," | box[3] = ", box[3])
                    global x
                    x = box[0] + 0.5*box[2]
                    global y
                    y = box[1] + 0.5*box[3]
                    #return x,y
                    #print(x,y)

    return img,objectInfo

if __name__ == "__main__":
    cap = cv2.VideoCapture(-1)
    cap.set(3,640)
    cap.set(4,480)
    #cap.set(10,70)
    ####### Initialize port
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout = 1)
    ser.flush()
    #######
    x = 0
    y = 0  
    while True:
        success,img = cap.read()
        result,objectInfo = getObjects(img, 0.45, 0.2, objects=['cup', 'bottle'])#chooses which one to detect
        print(objectInfo)
        #print(objectInfo.box[0])
        print(x,y)
        ###### Send X and Y
        ser.write("\n".encode('ascii'))
        ser.write(str(x).encode('ascii') + b"\n")
        ser.write(str(y).encode('ascii'))
        ######
        cv2.imshow("Output",img)
        cv2.waitKey(1)

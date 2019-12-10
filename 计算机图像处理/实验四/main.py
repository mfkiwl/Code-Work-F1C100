# encoding:utf-8

import requests
import base64
import cv2
import time
import json
import numpy as np

'''
easydl物体检测
'''

road1_num = 0
road2_num = 0
road3_num = 0
road4_num = 0

road1_flag = 0
road2_flag = 0
road3_flag = 0
road4_flag = 0
point1 = (140, 277)
point2 = (288,280)
point3 = ()
point4 = ()

class road():
    def __init__(self):
        #self.car = []
        self.num = 0
        self.car_distance_min = 0


def car_detect(img):
    res = requests.post('http://127.0.0.1:24401/', params={'threshold': 0.2},data=img).json()
    return res['results']


def img_process(img,pnt):
    # print(json.dumps(pnt,sort_keys=True,indent=4,separators=(',',':')))
    global road1_num, point1, point2
    for p in pnt:
        x1 = int(512 * p.get('x1'))
        y1 = int(288 * p.get('y1'))
        x2 = int(512 * p.get('x2'))
        y2 = int(288 * p.get('y2'))

        cv2.rectangle(img, (x1, y1),(x2 ,y2 ), (255, 255, 255), 1)
        #cv2.putText(img,"{}".format(road1_num),point1,cv2.FONT_HERSHEY_COMPLEX,2.0,(0,0,255))
        cv2.putText(img,"{}".format(road2_num), point2, cv2.FONT_HERSHEY_COMPLEX, 2.0, (0, 0, 255))
        cv2.circle(img,((x1+x2)//2,(y1+y2)//2),1, (255, 255, 255))
    return img

def main():
    cv2.namedWindow('output')
    count = 0
    count1 = 0
    count2 = 0
    count3 = 0
    count4 = 0
    flag1 = 0
    flag2 = 0
    flag3 = 0
    flag4 = 0
    for i in range(1,1564,3):
        img = cv2.imread('car_frame/frames_{:0>5d}.jpg'.format(i))
        res = car_detect(cv2.imencode('.jpg',img)[1].tostring())

        for p in res:
            x1 = int(512 * p.get('x1'))
            y1 = int(288 * p.get('y1'))
            x2 = int(512 * p.get('x2'))
            y2 = int(288 * p.get('y2'))

            contours = np.array([[x1,y1],[x2,y1],[x2,y2],[x1,y2]])
            cv2.fillPoly(img,pts=[contours],color=(0,0,0))
            k1 = img[278:288,60:210]
            k2 = img[278:288,211:380]
            k3 = img[278:288,450:512]
            k4 = img[150:160,430:512]
            cv2.rectangle(img,(60,278),(210,288),(0,255,0),1)
            cv2.rectangle(img,(211,278),(380,288),(0,255,0),1)
            cv2.rectangle(img,(450,278),(512,288),(0,255,0),1)
            cv2.rectangle(img,(430,150),(512,160),(0,255,0),1)

            n1 = len(k1[k1==0])
            n2 = len(k2[k2==0])
            n3 = len(k3[k3==0])
            n4 = len(k4[k4==0])

 #           print(len(n1),len(n2),len(n3),len(n4))

        if(n1>2000):
            flag1=1
        if(n2>2000):
            flag2=1
        if(n3>1000):
            flag3=1
        if(n4>1500):
            flag4=1

        if(n1<800 and flag1==1):
            count1=count1+1
            flag1=0
            cv2.rectangle(img,(60,278),(210,288),(0,0,255),1)
        if(n2<800 and flag2==1):
            count2=count2+1
            flag2=0
            cv2.rectangle(img,(211,278),(380,288),(0,0,255),1)
        if(n3<400 and flag3==1):
            count3=count3+1
            flag3=0
            cv2.rectangle(img,(450,278),(512,288),(0,0,255),1)
        if(n4<800 and flag4==1):
            count4=count4+1
            flag4=0
            cv2.rectangle(img, (430, 150), (512, 160), (0, 0, 255), 1)
        # print('1: {}, 2: {}, 3: {}, 4: {}'.format(count1,count2,count3,count4))
        cv2.putText(img, "1:{}".format(count1),(20,20),cv2.FONT_HERSHEY_COMPLEX, 1.0, (0, 0, 255))
        cv2.putText(img, "2:{}".format(count2),(20,45),cv2.FONT_HERSHEY_COMPLEX, 1.0, (0, 0, 255))
        cv2.putText(img, "3:{}".format(count3),(20,70),cv2.FONT_HERSHEY_COMPLEX, 1.0, (0, 0, 255))
        cv2.putText(img, "4:{}".format(count4),(20,95),cv2.FONT_HERSHEY_COMPLEX, 1.0, (0, 0, 255))

        cv2.putText(img, "{}".format(len(res)),(462,83),cv2.FONT_HERSHEY_COMPLEX, 1.0, (0, 0, 255))
        cv2.imwrite('res/frames_{:0>5d}.jpg'.format(i), img)
        cv2.imshow('output',img)
        cv2.waitKey(200)


    cv2.waitKey()

main()

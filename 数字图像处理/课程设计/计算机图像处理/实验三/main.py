import cv2
import numpy as np
import math

def obtain_objectcoordinate(center,width,height):
    x1 = center[1]-math.floor((width-1)/2)
    y1 = center[0]-math.floor((height-1)/2)
    x2 = x1+width-1
    y2=y1+height-1
    return(x1,y1,x2,y2)

y_0 = [65,145]          # 初始化目标位置
rect_width = 17
w_t = rect_width        # 目标窗口的宽度 w_t/2 = 8
h_t = rect_width        # 目标窗口的高度 h_t/2 = 10
W = rect_width          # 跟踪窗口的宽度 W/2 = 16
H = rect_width          # 跟踪窗口的高度 H/2 = 17
w_bg = rect_width       # 背景窗口的宽度 w_bg/2 = 20
h_bg = rect_width       # 背景窗口的高度 h_bg/2 = 23

# 截断 Gauss 核参数
rda = 1000
beda = 0.003

directory = 'TableTennis'
imgfile1='stennisball0.bmp'
X = cv2.imread('{}/{}'.format(directory,imgfile1))
img_height,img_width,_ = X.shape
img_width /= 3

x1_t,y1_t,x2_t,y2_t = obtain_objectcoordinate(y_0,w_t,h_t)
sum_q = 0
histo = np.zeros((16,16,16))
# 1) 计算目标窗口 rect*rect 在RGB特征空间的q_u统计
for i in range(y1_t,y2_t+1):
    for j in range(x1_t,x2_t+1):
        k=1
        R=math.floor(X(i,j,1))
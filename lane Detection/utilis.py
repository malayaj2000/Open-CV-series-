import cv2
import numpy as np
import matplotlib.pyplot as plt

def warping(frame,points,w,h,inv=False):
    pts1 = np.float32(points)
    pts2 = np.float32([[0,0],[w,0],[0,h],[w,h]])
    if inv:
        matrix =  cv2.getPerspectiveTransform(pts2,pts1)
    else:
        matrix =  cv2.getPerspectiveTransform(pts1,pts2)
    warpframe = cv2.warpPerspective(frame,matrix,(w,h))
    return warpframe
def blur(frame):
    blur_f = cv2.GaussianBlur(frame, (7,7),0)
    return blur_f
def hsv_mask_y(frame):
    hue_min = 0
    sat_min = 84
    value_min = 160
    value_max = 255
    hue_max = 177
    sat_max = 255
    low = (hue_min,sat_min,value_min)
    high = (hue_max,sat_max, value_max)
    mask= cv2.inRange(frame,low,high)
    return mask
def hsv_mask_w(frame):
    max_thres=255
    min_thres=211
    gray = cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
    thres = cv2.threshold(gray,min_thres,max_thres,cv2.THRESH_BINARY)[1]
    return thres
def masking(mask_yellow ,mask_white):
    mask = mask_yellow | mask_white
    return mask

def pross_mask(frame,invwarp):
    imgRes = np.zeros_like(frame)
    imgRes[:, :, :] = 255, 0, 0
    imgRes = cv2.bitwise_and(imgRes, imgRes, mask=invwarp)
    img = cv2.bitwise_not(invwarp)
    img = cv2.bitwise_and(frame, frame, mask=img)
    output = img | imgRes
    return output
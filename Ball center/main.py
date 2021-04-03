import cv2
import numpy as np
def nothing(x):
    pass
#cv2.namedWindow("tracker")
cv2.namedWindow("image")
cv2.resizeWindow("image",700,512)
#cv2.createTrackbar("hue_min","tracker",0,179,nothing)
#cv2.createTrackbar("hue_max","tracker",179,179,nothing)
#cv2.createTrackbar("sat_min","tracker",0,255,nothing)
#cv2.createTrackbar("sat_max","tracker",255,255,nothing)
#cv2.createTrackbar("value_min","tracker",0,255,nothing)
#cv2.createTrackbar("value_max","tracker",255,255,nothing)
#cv2.createTrackbar("min_thres","tracker",0,255,nothing)
#cv2.createTrackbar("max_thres","tracker",255,255,nothing)
#img = cv2.imread("Movies & TV 25-Mar-21 9_45_25 AM.png")
#img_2 = img.copy()
cap = cv2.VideoCapture("red ball green background.mp4")
ret,frame = cap.read()
while ret:
  #  hue_min = cv2.getTrackbarPos("hue_min","tracker")
  #  sat_min = cv2.getTrackbarPos("sat_min","tracker")
    # value_min = cv2.getTrackbarPos("value_min","tracker")
   # value_max = cv2.getTrackbarPos("value_max","tracker")
   # hue_max = cv2.getTrackbarPos("hue_max","tracker")
   # sat_max = cv2.getTrackbarPos("sat_max","tracker")
   # min_thres = cv2.getTrackbarPos("min_thres","tracker")
    # max_thres = cv2.getTrackbarPos("max_thres","tracker")
    ret,frame = cap.read()
    if(ret==False):
        break
    hsv = cv2.cvtColor(frame.copy(), cv2.COLOR_BGR2HSV)
    blur  = cv2.GaussianBlur(hsv.copy(),(9,9),1,1)
    hue_min=27
    hue_max=179
    sat_min=62
    sat_max=241
    value_min=42
    value_max=255
    min_thres = np.array([hue_min,sat_min,value_min])
    max_thres = np.array([hue_max, sat_max, value_max])
    thrs = cv2.inRange(blur, min_thres, max_thres)
    mask = cv2.bitwise_and(frame,frame,mask= thrs)
    canny = cv2.Canny(thrs,100,200)
    #cnt,hierarchy  = cv2.findContours(canny,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
    #cnt = sorted(cnt,key= cv2.contourArea,reverse=True)[:2]
    #centroid = []
    #for c in cnt:
        #epsilon = 0.005 * cv2.arcLength(c, True)
        #approx = cv2.approxPolyDP(c, epsilon, True)
        #cv2.drawContours(frame,[approx],0,(255,255,84),2)
        #M= cv2.moments(c)
        #if(M["m10"]==0 or M["m01"]==0):
            #continue
        #else:
            #cx = int(M["m10"]/M["m00"])
            #cy = int(M["m01"]/M["m00"])
            #centroid.append((cx,cy))
            #cv2.circle(frame, (cx, cy), 7, (0, 255, 255), -1)x
    #if centroid:
        #for c in centroid:
            #if(abs(x1-x2)<50 and abs(y1-y2)<50):
                #x+=x1+x2
            #cv2.circle(frame,(c[0],c[1]),7,(0,255,255),-1)
    circles=cv2.HoughCircles(canny,cv2.HOUGH_GRADIENT,1,20,param1=50,param2=30,minRadius=0,maxRadius=0)

    if circles is not None:
        circles = np.uint16(np.around(circles))
        circle = sorted(circles[0],key=lambda x:x[2],reverse=True)[:2]
        #print(circle)
        if (len(circle) == 2 and abs(circle[0][0] - circle[1][0]) < 50):
            cv2.circle(frame,(circle[0][0],circle[0][1]),circle[0][2],(0,100,100),3)
            cv2.circle(frame, (circle[0][0], circle[0][1]), 1, (0, 100, 100), 3)
        else:
            for i in circle:
                center = (i[0], i[1])
            # circle center
                cv2.circle(frame, center, 1, (0, 100, 100), 3)
            # circle outline
                radius = i[2]
                cv2.circle(frame, center, radius, (255, 0, 255), 3)
    cv2.imshow("image",frame)
    cv2.imshow("threshold",mask)
    key = cv2.waitKey(10)
    
    if(key == 27):
        break

cv2.destroyAllWindows()
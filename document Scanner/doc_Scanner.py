import cv2
import numpy as np
import imutils
import argparse

ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", required=True, help="path of image")
arg = vars(ap.parse_args())
img1 = cv2.imread(arg['image'])
ratio=img1.shape[0]/500
orgi=img1.copy()
img1=imutils.resize(img1,height=500)
gray1=cv2.cvtColor(img1,cv2.COLOR_BGR2GRAY)
blur = cv2.GaussianBlur(gray1,(5,5),0)
edge=cv2.Canny(blur,75, 200)
cnt=cv2.findContours(edge,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
cnt= imutils.grab_contours(cnt)
cnt=sorted(cnt,key=cv2.contourArea,reverse=True)[:3]
points =0
for c in cnt:
    prei=cv2.arcLength(c,True)
    approx=cv2.approxPolyDP(c,0.1*prei,True)
    if len(approx)==4:
        points=approx
        print("done")
        break
pt1=np.float32([points[0][0],points[3][0],points[1][0],points[2][0]])
y,x=img1.shape[:2]
pt2=np.float32([[0,0],[x,0],[0,y],[x,y]])
matrix=cv2.getPerspectiveTransform(pt1,pt2)

new=cv2.warpPerspective(img1,matrix,(x,y))
new_gray=cv2.cvtColor(new,cv2.COLOR_BGR2GRAY)
final=cv2.adaptiveThreshold(new_gray, 255,1,1,5,3)
final1=cv2.bitwise_not(final)
final2=cv2.medianBlur(final1,1)
cv2.imshow("img",final2)
cv2.waitKey(0)
cv2.destroyAllWindows()
print("DO YOU WANT TO SAVE THE GENERATED IMAGE \n [y/n]")
ans=input()
if(ans=='y'):
    cv2.imwrite("doc_Scanner.jpg",final2)
    print("saved")

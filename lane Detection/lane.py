import cv2
import numpy as np
import matplotlib.pyplot as plt
import utilis
if __name__ == '__main__':
    key = 0
    currlistYellow =[]
    avgVal = 10
    path = "5.mp4"
    w=1000
    h=700

    wT = 407
    hT = 434
    wB = 145
    hB = 613
    cap = cv2.VideoCapture(path)
    ret,frame = cap.read()
    while(1):
        ret,frame = cap.read()
        if(not ret):
            cap = cv2.VideoCapture(path)
            continue
        frame=cv2.resize(frame,(w,h))
        h,w,c= frame.shape

        points = np.float32([(wT,hT),(w-wT,hT),(wB,hB),(w-wB,hB)])

        warp = utilis.warping(frame,points,w,h)

        hsv = cv2.cvtColor(warp.copy(),cv2.COLOR_BGR2HSV)

        blur_f = utilis.blur(hsv)

        mask_yellow = utilis.hsv_mask_y(blur_f)

        mask_white = utilis.hsv_mask_w(warp.copy())

        mask_f_white = cv2.bitwise_and(warp,warp,mask = mask_white)

        mask_f_yellow = cv2.bitwise_and(warp,warp,mask = mask_yellow)

        mask =utilis.masking(mask_yellow ,mask_white)

        invwarp = utilis.warping(mask, points, w, h, inv=True)

        output=utilis.pross_mask(frame,invwarp)

        key = cv2.waitKey(10)
        cv2.imshow("video", output)
        if(key == 27):
            break
    cv2.destroyAllWindows()
    cap.release()

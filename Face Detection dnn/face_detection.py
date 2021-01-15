import cv2
import argparse
import numpy as np
ap = argparse.ArgumentParser()
ap.add_argument("-i","--image",required=True,help="image path")
ap.add_argument("-p", "--prototext", required=True,
help="path to Caffe 'deploy' prototxt file")
ap.add_argument("-m", "--model", required=True,
help="path to Caffe pre-trained model")
ap.add_argument("-c", "--confidence", type=float, default=0.5,
help="minimum probability to filter weak detections")
args = vars(ap.parse_args())
net = cv2.dnn.readNetFromCaffe(args["prototext"], args['model'])
img = cv2.imread(args["image"])
(h, w) = img.shape[:2]
blob = cv2.dnn.blobFromImage(cv2.resize(img, (300, 300)), 1, (300, 300), (104, 177, 123))
net.setInput(blob)
detection=net.forward()
for i in range(0, detection.shape[2]):
    confidence = detection[0, 0, i, 2]

    if confidence > args["confidence"]:
        box = detection[0, 0, i, 3:7] * np.array([w, h, w, h])
        (x1, y1, x2, y2) = box.astype("int")

        text = "{:.2f}%".format(confidence * 100)
        y = y1 - 10 if y1 - 10 > 10 else y1 + 10
        cv2.rectangle(img, (x1, y1), (x2, y2), (154, 255, 25), 2)
        cv2.putText(img, text, (x1, y), cv2.FONT_HERSHEY_COMPLEX, 0.45, (0, 255, 255), 2)
cv2.imshow("IMG", img)
cv2.waitKey(0)
cv2.destroyAllWindows()


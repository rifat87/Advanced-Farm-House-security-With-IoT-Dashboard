import os
import pickle
import numpy as np
import cv2 as cv
import face_recognition
import requests

cap = cv.VideoCapture(0)
cap.set(3, 640)
cap.set(4, 480)

url = "http://192.168.137.223/cam-hi.jpg"
#importing mode images into a list

# for loading the image
imgBackground = cv.imread('Resources/background.png')

# now we are going to access the modes using its path to array list or import the mode images
folderModePath = 'Resources/Modes'
modePathList = os.listdir(folderModePath)
imgModeList = []
for path in modePathList:
    imgModeList.append(cv.imread(os.path.join(folderModePath,path)))  # this will append all the pics path to the imgModeList array
# print(modePathList)#this will print all the pics path in modes folder
#print(len(imgModeList))

#load the encoding file
print("laoding encode file.......")

file = open('EncodeFile.p', 'rb')
encodeListKnownWithIds = pickle.load(file)
file.close()
encodeListKnown, studentIds = encodeListKnownWithIds
#print(studentIds) for checking student ids
print("Encode file laoded......")


while True:
    #we will squish the image to 1/4 of the size
    success, img = cap.read()
    # #response = requests.get(url)
    # response = requests.get(url)
    #
    #     # Read the content of the response and convert it to a NumPy array
    # img_array = np.frombuffer(response.content, dtype=np.uint8)
    #
    # # Decode the image as an OpenCV image
    # img = cv.imdecode(img_array, cv.IMREAD_COLOR)

    # Process the image here, for example, detecting objects or drawing on the image.

    # Display the image
    # cv.imshow("Video Stream", img)

    # # Exit the loop if the 'q' key is pressed
    # if cv.waitKey(1) & 0xFF == ord('q'):
    #     break
    #img_array = np.array(bytearray(response.content), dtype=np.uint8)
    #img = cv.imdecode(img_array, -1)

    imgS = cv.resize(img, (0, 0), None, 0.25, 0.25) #this will resize the image
    imgS = cv.cvtColor(imgS, cv.COLOR_BGR2RGB)#convert the image color

    #now we will feed the value to facerecognition system
    faceCurFrame = face_recognition.face_locations(imgS)
    encodeCurFrame = face_recognition.face_encodings(imgS, faceCurFrame)

    # now we are going to attach both webcam and attendence background
    imgBackground[162:162 + 480, 55:55 + 640] = img
    imgBackground[44:44 + 633, 808:808 + 414] = imgModeList[3]

    #now we are going to loop through the encodingCurFrame
    #by using zip we can run through both encodeCurFrame and faceCurFrame otherWise we have to write 2 for loop for it
    for encodeFace, faceLoc in zip(encodeCurFrame, faceCurFrame):
           matches = face_recognition.compare_faces(encodeListKnown, encodeFace)
           faceDis = face_recognition.face_distance(encodeListKnown, encodeFace)
           print("matches:", matches)
           print("faceDis:", faceDis)

           #now we will get the index number of the lowest faceDis using numpy
           matchIndex = np.argmin(faceDis)
           print("matchIndex:", matchIndex)

           if matches[matchIndex]:
                print("Known Face Detected")
    #encode from encodeCurFrame goes to encoFace, and faceCurFrame goes to faceLoc

    # cv.imshow("Webcam", img)
    cv.imshow("Face Recognition", imgBackground)
    cv.waitKey(1)

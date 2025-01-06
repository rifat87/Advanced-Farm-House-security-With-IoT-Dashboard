import cv2 as cv
import face_recognition
import pickle
import os

# importig the student images
folderPath = 'Images'
PathList = os.listdir(folderPath)
print(PathList)  # this will print all the paths of the photo in a list

imgList = []
studentIds = []
for path in PathList:
    imgList.append(cv.imread(os.path.join(folderPath, path)))
    #print(path)
    #print(os.path.splitext(path)[0]) #this will split the path of photo [123.png to [123, .png] ] and to get the first element put [0] after path
    studentIds.append(os.path.splitext(path)[0])
print(studentIds)

#encoding function
def findEncodings(imagesList):
    encodeList = []
    #we are going to loop through every images and encode every single image
    for img in imagesList:
        img = cv.cvtColor(img, cv.COLOR_BGR2RGB) #we need to convert color from BGR to RGB, cause opencv uses BGR and facerecognition use RGB
        encode = face_recognition.face_encodings(img)[0]#0 for first element of this
        encodeList.append(encode)


    return encodeList

print("Encoding Started...")
encodeListKnown = findEncodings(imgList)
encodeListKnownWithIds = [encodeListKnown, studentIds]
#print(encodeListKnown)#it will start the encoding and give the values in a array
print("Encoding Completed")
#now we hvae to save it to a pickle file and so that we can import it when we use the webcam
#craeting pickle file

#for giving some permissions use wb
file = open("EncodeFile.p", 'wb')
pickle.dump(encodeListKnownWithIds,file)
file.close()
print("File Saved")
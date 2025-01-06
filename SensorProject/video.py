import cv2 as cv
import serial

# Establish serial connection
ser = serial.Serial('COM4', 115200)

# URL of the video stream
url = "http://192.168.137.187"

# Open a VideoCapture object with the URL
cap = cv.VideoCapture(0)

# Check if the video stream was opened successfully
if not cap.isOpened():
    print("Error: Could not open video stream.")
    exit()

while True:
    # Read a frame from the video stream
    ret, frame = cap.read()

    # If the frame was read successfully, display it
    if ret:
        cv.imshow('Video', frame)
        value1 = 10
        value2 = 87

        print("The angle is = ", value1)
        print(" The distance is : ", value2)

        data = f"{value2},{value1}\n"
        # data = "{} {}\n".format(value1, value2)

        ser.write(data.encode())
        response = ser.readline().decode().strip()
        if response:
            print("Arduino:", response)
        cv.waitKey(1000)
    # Break the loop if the 'q' key is pressed
    if cv.waitKey(1) & 0xFF == ord('q'):
        break

# Release the VideoCapture and close all OpenCV windows
cap.release()
cv.destroyAllWindows()
ser.close()
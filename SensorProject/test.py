import cv2
import requests
import numpy as np

# URL of the image stream
url_base = "http://192.168.137.93"
url_endpoint = "/cam-hi.jpg"
url = "http://192.168.137.93/cam-hi.jpg"
while True:
    # Construct the complete image URL
    full_url = url_base + url_endpoint

    # Fetch the image from the URL using requests
    response = requests.get(url)

    if response.status_code == 200:
        # Read the content of the response and convert it to a NumPy array
        img_array = np.frombuffer(response.content, dtype=np.uint8)

        # Decode the image as an OpenCV image
        img = cv2.imdecode(img_array, cv2.IMREAD_COLOR)

        # Process the image here, for example, detecting objects or drawing on the image.

        # Display the image
        cv2.imshow("Video Stream", img)

    # Exit the loop if the 'q' key is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Close OpenCV window
cv2.destroyAllWindows()

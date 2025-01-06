#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>

// Use Serial2 on the ESP32 for communication with Arduino
#define RX2 16  // Define RX pin for Serial2
#define TX2 17  // Define TX pin for Serial2

const char* serverName = "http://192.168.24.15:5000/sensor-data"; 

// Sensor data to be sent
const char* writeApiKey = "fe09d309f4cc48dcefae52459fd86751";  // Replace with your actual Write API Key
const char* sensorName = "DHT";
int sensorValue = 10;


HardwareSerial arduinoSerial(2);  // Use Serial2 for communication with Arduino

const char* ssid = "Jalish";       // Local Wi-Fi SSID
const char* password = "12345678"; // Local Wi-Fi Password
const char* ap_ssid = "No_Network";         // AP SSID
const char* ap_password = "rifat2001005";    // AP Password

// Timer variables
unsigned long lastSendTime = 0; // Stores the last send time
const unsigned long interval = 5000; // 5-second interval

WiFiServer server(80);
Servo myServo;
// Variables to store sensor data
int humidity = 0;
int temperature = 0;
int gasSensorValue = 0;

  // Construct the URL with actual values
// String baseurl = "http://192.168.43.165:5000/sendespdata/";

void setup() {
  Serial.begin(115200);
  myServo.attach(5);
  delay(10);

   // Connect to local Wi-Fi (Station Mode)
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to local Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConnected to local Wi-Fi!");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());

  WiFi.softAP(ap_ssid, ap_password);

  IPAddress apIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(apIP);

  server.begin();
  arduinoSerial.begin(115200, SERIAL_8N1, RX2, TX2);  // Initialize Serial2
}

void loop() {
  // Handle data from the Arduino
  if (arduinoSerial.available()) {
    String data = arduinoSerial.readStringUntil('\n');  // Read until newline
    data.trim(); // Remove extra spaces or newlines
    Serial.println("Received from Arduino: " + data);

    // Check if the data follows the expected format
    if (data.startsWith("Humidity:") && data.indexOf(", Temperature:") != -1) {
      int humStart = data.indexOf("Humidity: ") + 10;
      int humEnd = data.indexOf(", Temperature: ");
      int tempStart = humEnd + 15;
      int tempEnd = data.indexOf(", Gas Sensor: ");
      int gasStart = tempEnd + 13;

      if (humStart != -1 && humEnd != -1 && tempStart != -1 && tempEnd != -1 && gasStart != -1) {
        humidity = data.substring(humStart, humEnd).toInt();
        
        temperature = data.substring(tempStart, tempEnd).toInt();
        gasSensorValue = data.substring(gasStart).toInt();

        Serial.print("Parsed Humidity: ");
        Serial.println(humidity);
        Serial.print("Parsed Temperature: ");
        Serial.println(temperature);
        Serial.print("Parsed Gas Sensor Value: ");
        Serial.println(gasSensorValue);
      } else {
        Serial.println("Failed to parse data.");
      }
    } else {
      Serial.println("Unexpected data format.");
    }
  }

  // Handle Wi-Fi client requests
  WiFiClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        String clientData = client.readStringUntil('\n');
        clientData.trim();
        Serial.print("Received data from Wi-Fi client: ");
        Serial.println(clientData);

        // Parse the received data
      int commaIndex = clientData.indexOf(','); // Corrected this line
      if (commaIndex != -1) {
        String value1String = clientData.substring(0, commaIndex); // Get first value
        String value2String = clientData.substring(commaIndex + 1); // Get second value
        
        int value1 = value1String.toInt(); // Convert to integer
        int value2 = value2String.toInt(); // Convert to integer

        Serial.print("Value 1: ");
        Serial.println(value1);
        Serial.print("Value 2: ");
        Serial.println(value2);
        // Send the received data to Arduino
        // arduinoSerial.println(clientData);
        ///
        if (value1 == 1) {
            myServo.write(90); // Move servo to 90 degrees
            delay(5000);       // Wait for 5 seconds
            myServo.write(0);  // Optionally return the servo to its initial position
          }
      }
    }
    }
    client.stop(); // Disconnect the client
    Serial.println("Client disconnected");
  }

  

  // Send DHT data via HTTP POST
  HTTPClient http; // Initialize HTTPClient here
  http.begin(serverName);

  // Set the request headers
  http.addHeader("Content-Type", "application/json");

  // Use actual humidity or temperature value
  String jsonData = "{\"writeApiKey\":\"" + String(writeApiKey) + "\",\"sensorName\":\"" + String(sensorName) + "\",\"value\":" + String(gasSensorValue) + "}";

  // Send the POST request
  int httpResponseCode = http.POST(jsonData);
  Serial.println("Data is snet");
  // Handle the response
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString();
    Serial.println("Response: " + response);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  // End the connection
  http.end();
}

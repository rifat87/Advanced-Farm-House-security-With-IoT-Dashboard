#include <WiFi.h>

const char* ssid = "No_Network";
const char* password = "rifat2001005";
const char* serverIP = "192.168.4.1";  // IP address of ESP8266 AP
const int serverPort = 80;

int value1 ;
int value2 ;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to Wi-Fi network
  // Serial.println();
  // Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Serial.println("WiFi connected");
  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (!client.connected()) {
    // Serial.println("Connecting to server...");
    if (client.connect(serverIP, serverPort)) {
      Serial.println("Connected to server");
      // client.println("Hello wrold");
      if(Serial.available()){
        String data = Serial.readStringUntil('\n');
  
  // Parse the received data
        int commaIndex = data.indexOf(',');
        if (commaIndex != -1) {
        String value2String = data.substring(0, commaIndex);
        String value1String = data.substring(commaIndex + 1);
        value2 = value2String.toInt();
        value1 = value1String.toInt();
        Serial.println("Value: ");
        Serial.println(value2);

        String data = String(value1) + "," + String(value2);
        // client.print("The angle is: ");
        // Send data to ESP8266
        client.println(data);
        
        String response = "Received: " + data;
        Serial.println(response);
      }
    }


      delay(1000);
      // Read response from ESP8266
      while (client.available()) {
        String response = client.readStringUntil('\n');
        Serial.print("Response from ESP8266: ");
        Serial.println(response);
      }
      client.stop();
    } else {
      Serial.println("Connection failed");
    }
  }
}

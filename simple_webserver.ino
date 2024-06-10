#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Realme 9pro 5G";
const char* password = "gowthamn16";
const char* serverAddress = "169.254.32.241";
const int serverPort = 3003; // Change to the port used by your Node.js server

ESP8266WebServer server(80);
WiFiClient client;
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("Local IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, []() {
    // Your HTML form to collect data from the user
    String html = "<html><body>";
    html += "<form action='/submit' method='POST'>";
    html += "Data: <input type='text' name='data'><br>";
    html += "<input type='submit' value='Submit'>";
    html += "</form></body></html>";
    server.send(200, "text/html", html);
  });

  server.on("/submit", HTTP_POST, []() {
    // Handle the submitted data and send it to your Node.js server
    if (server.hasArg("data")) {
      String data = server.arg("data");
      // Send data to your Node.js server
      sendDataToNodeJS(data);
      server.send(200, "text/plain", "Data sent to server: " + data);
    } else {
      server.send(400, "text/plain", "Bad Request");
    }
  });

  server.begin();
}

void loop() {
  server.handleClient();
}

void sendDataToNodeJS(String data) {
  HTTPClient http;
  
  http.begin(client, "http://" + String(serverAddress) + ":" + String(serverPort) + "/nodemcu-data"); // Updated URL
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String postData = "data=" + data;
  int httpResponseCode = http.POST(postData);
  http.end();
  Serial.println("Data sent to Node.js server with response code: " + httpResponseCode);
}

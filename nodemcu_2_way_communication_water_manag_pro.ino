
#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

#include <WiFiUdp.h>
#include <NTPClient.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org",19800);

//Provide the token generation process info.
#include <addons/TokenHelper.h>

//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "IQOO Z3 RN1015"
#define WIFI_PASSWORD "yrge1423"

//For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyCTNtHX-G3JPNy7-zSXXo-6OncyL6qgiJ0"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://es-lab-75371-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "kannan.m21@iiits.in"
#define USER_PASSWORD "123456789"

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
String appdata = "";


unsigned long count = 0;

String getCurrentTimeString()
{
timeClient.update();
time_t now = timeClient.getEpochTime();
struct tm *timeinfo;
timeinfo = localtime(&now);
char buffer[30];
strftime(buffer, sizeof(buffer), "%A, %Y-%m-%d %H:%M:%S", timeinfo);
return String(buffer);
}


void setup()
{

Serial.begin(115200);

WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
Serial.print("Connecting to Wi-Fi");
while (WiFi.status() != WL_CONNECTED)
{
Serial.print(".");
delay(300);
}
Serial.println();
Serial.print("Connected with IP: ");
Serial.println(WiFi.localIP());
Serial.println();

Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

/* Assign the api key (required) */
config.api_key = API_KEY;
auth.user.email = USER_EMAIL;
auth.user.password = USER_PASSWORD;

config.database_url = DATABASE_URL;

config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

Firebase.begin(&config, &auth);

Firebase.reconnectWiFi(true);

Firebase.setDoubleDigits(5);

timeClient.begin();
//timeClient.setTimeOffset(19800); // UTC+5:30 for India
}




void loop()
{
float v1 = random(1,10);
float v2 = random(11,20);
float v3 = random(4,50);
float v4 = random(51,60);

if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0))
{
sendDataPrevMillis = millis();

Serial.printf("Set User1... %s\n", Firebase.setString(fbdo, F("/User1/Name"), "User1") ? "ok" : fbdo.errorReason().c_str());

Serial.printf("Get User1... %s\n", Firebase.getString(fbdo, F("/User1/Name")) ? String(fbdo.to<String>()).c_str() : fbdo.errorReason().c_str());

Serial.printf("Set UserID1... %s\n", Firebase.setString(fbdo, F("/User1/UserId"), "Uid1") ? "ok" : fbdo.errorReason().c_str());

Serial.printf("Get UserID1... %s\n", Firebase.getString(fbdo, F("/User1/UserId")) ? String(fbdo.to<String>()).c_str() : fbdo.errorReason().c_str());

Serial.printf("Set Variable 1... %s\n", Firebase.setFloat(fbdo, F("/User1/node1/Variable1"), v1) ? "ok" : fbdo.errorReason().c_str());

Serial.printf("Get Variable 1... %s\n", Firebase.getFloat(fbdo, F("/User1/node1/Variable1")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

Serial.printf("Set Variable 2... %s\n", Firebase.setFloat(fbdo, F("/User1/node1/Variable2"), v2) ? "ok" : fbdo.errorReason().c_str());

Serial.printf("Get Variable 2... %s\n", Firebase.getFloat(fbdo, F("/User1/node1/Variable2")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

Serial.printf("Set Variable 3... %s\n", Firebase.setFloat(fbdo, F("/User1/node2/Variable3"), v3) ? "ok" : fbdo.errorReason().c_str());

Serial.printf("Get Variable 3... %s\n", Firebase.getFloat(fbdo, F("/User1/node2/Variable3")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

Serial.printf("Set Variable 4... %s\n", Firebase.setFloat(fbdo, F("/User1/node2/Variable4"), v4) ? "ok" : fbdo.errorReason().c_str());

Serial.printf("Get Variable 4... %s\n", Firebase.getFloat(fbdo, F("/User1/node2/Variable4")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());


//For the usage of FirebaseJson, see examples/FirebaseJson/BasicUsage/Create_Parse_Edit.ino
FirebaseJson json;
// json.set(".sv","timestamp");
json.set("time",getCurrentTimeString());
//Serial.printf("Set json... %s\n", Firebase.set(fbdo, F("/User1"), json) ? "ok" : fbdo.errorReason().c_str());
// Serial.printf("Update node... %s\n", Firebase.updateNode(fbdo, F("/test/json/value/round"), json) ? "ok" : fbdo.errorReason().c_str());
Serial.println();
count++;
}



//**************************Read from Cloud*********************************//
if (Firebase.getString(fbdo, "/User1/boolean"))
{
appdata = fbdo.stringData();
Serial.print("App Data = ");
Serial.println(appdata);
Serial.println();
}
else
{
Serial.println(fbdo.errorReason());
}

}

#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif


//Provide the token generation process info.
#include <addons/TokenHelper.h>

//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>



/* 1. Define the WiFi credentials */
#define WIFI_SSID "IIITS_Student"
#define WIFI_PASSWORD "iiit5@2k18"

//For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyDLvX0NA0JMvHFS7Ow6tdLJ5sxfjHwVwqk"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://watermanagementsystem-92386-default-rtdb.asia-southeast1.firebasedatabase.app/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "adesolasamuel2018@gmail.com"
#define USER_PASSWORD "esp8266demo"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;


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

 
}

void loop()
{
  float h = 1222;
  float t = 12322;

  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    Serial.printf("Set value1... %s\n", Firebase.setFloat(fbdo, F("/test/value1"), t) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get value1... %s\n", Firebase.getFloat(fbdo, F("/test/value1")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

    Serial.printf("Set value2... %s\n", Firebase.setDouble(fbdo, F("/test/value2"), h) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get value2... %s\n", Firebase.getDouble(fbdo, F("/test/value2")) ? String(fbdo.to<double>()).c_str() : fbdo.errorReason().c_str());

        //For the usage of FirebaseJson, see examples/FirebaseJson/BasicUsage/Create_Parse_Edit.ino
    FirebaseJson json;

    if (count == 0)
    {
      json.set("value/round/" + String(count), F("cool!"));
      json.set(F("value/ts/.sv"), F("timestamp"));
      Serial.printf("Set json... %s\n", Firebase.set(fbdo, F("/test/json"), json) ? "ok" : fbdo.errorReason().c_str());
    }
    else
    {
      json.add(String(count), "smart!");
      Serial.printf("Update node... %s\n", Firebase.updateNode(fbdo, F("/test/json/value/round"), json) ? "ok" : fbdo.errorReason().c_str());
    }
    
    Serial.println();
    
  
    count++;
  }
}

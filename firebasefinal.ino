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
#define WIFI_SSID "IIITS_Student"
#define WIFI_PASSWORD "iiit5@2k18"

//For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyDLvX0NA0JMvHFS7Ow6tdLJ5sxfjHwVwqk"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://watermanagementsystem-92386-default-rtdb.asia-southeast1.firebasedatabase.app/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "eswar.k21@iiits.in"
#define USER_PASSWORD "icpsproject"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;

String getCurrentTimeString() {
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

// Define an array of usernames and user IDs
const char* usernames[] = {"User1", "User2", "User3"};
const char* userIds[] = {"uid1", "uid2", "uid3"};




void loop()
{
  float Tds1 = 1222;
  float Turbidity1 = 12.22;
  float Ph1 = 11;
  float Star1 = 1;

  float Tds2 = 1222;
  float Turbidity2 = 12.22;
  float Ph2 = 11;
  float Star2 = 1;
  
 
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
     Serial.printf("Set User1... %s\n", Firebase.setString(fbdo, F("/User1/Name"), usernames[0]) ? "ok" : fbdo.errorReason().c_str());

     Serial.printf("Get User1... %s\n", Firebase.getString(fbdo, F("/User1/Name")) ? String(fbdo.to<String>()).c_str() : fbdo.errorReason().c_str());


     Serial.printf("Set UserID1... %s\n", Firebase.setString(fbdo, F("/User1/UserId"), userIds[0]) ? "ok" : fbdo.errorReason().c_str());

     Serial.printf("Get UserID1... %s\n", Firebase.getString(fbdo, F("/User1/UserId")) ? String(fbdo.to<String>()).c_str() : fbdo.errorReason().c_str());


    Serial.printf("Set Tds... %s\n", Firebase.setFloat(fbdo, F("/User1/node1/Tds"), Tds1) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get Tds... %s\n", Firebase.getFloat(fbdo, F("/User1/node1/Tds")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());


    Serial.printf("Set Turbidity... %s\n", Firebase.setFloat(fbdo, F("/User1/node1/Turbidity"), Turbidity1) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get Turbidity... %s\n", Firebase.getFloat(fbdo, F("/User1/node1/Turbidity")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

    
    Serial.printf("Set Ph... %s\n", Firebase.setFloat(fbdo, F("/User1/node1/Ph"), Ph1) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get Ph... %s\n", Firebase.getFloat(fbdo, F("/User1/node1/Ph")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

   
    Serial.printf("Set Star... %s\n", Firebase.setFloat(fbdo, F("/User1/node1/Star"),Star1) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get Star... %s\n", Firebase.getFloat(fbdo, F("/User1/node1/Star")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

    
    Serial.printf("Set Tds... %s\n", Firebase.setFloat(fbdo, F("/User1/node2/Tds"), Tds2) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get Tds... %s\n", Firebase.getFloat(fbdo, F("/User1/node2/Tds")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());


    Serial.printf("Set Turbidity... %s\n", Firebase.setFloat(fbdo, F("/User1/node2/Turbidity"), Turbidity2) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get Turbidity... %s\n", Firebase.getFloat(fbdo, F("/User1/node2/Turbidity")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

    
    Serial.printf("Set Ph... %s\n", Firebase.setFloat(fbdo, F("/User1/node2/Ph"), Ph2) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get Ph... %s\n", Firebase.getFloat(fbdo, F("/User1/node2/Ph")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

   
    Serial.printf("Set Star... %s\n", Firebase.setFloat(fbdo, F("/User1/node2/Star"),Star2) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get Star... %s\n", Firebase.getFloat(fbdo, F("/User1/node2/Star")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

     delay(3000);
   //For the usage of FirebaseJson, see examples/FirebaseJson/BasicUsage/Create_Parse_Edit.ino
    FirebaseJson json;
   // json.set(".sv","timestamp");
    json.set("time",getCurrentTimeString());
    
    Serial.printf("Set json... %s\n", Firebase.set(fbdo, F("/User1"), json) ? "ok" : fbdo.errorReason().c_str());
   // Serial.printf("Update node... %s\n", Firebase.updateNode(fbdo, F("/test/json/value/round"), json) ? "ok" : fbdo.errorReason().c_str());
 
    


     Serial.printf("Set User2... %s\n", Firebase.setString(fbdo, F("/User2/Name"), usernames[1]) ? "ok" : fbdo.errorReason().c_str());

     Serial.printf("Get User2... %s\n", Firebase.getString(fbdo, F("/User2/Name")) ? String(fbdo.to<String>()).c_str() : fbdo.errorReason().c_str());


     Serial.printf("Set UserID2... %s\n", Firebase.setString(fbdo, F("/User2/UserId"), userIds[1]) ? "ok" : fbdo.errorReason().c_str());

     Serial.printf("Get UserID2... %s\n", Firebase.getString(fbdo, F("/User2/UserId")) ? String(fbdo.to<String>()).c_str() : fbdo.errorReason().c_str());


    Serial.printf("Set Tds... %s\n", Firebase.setFloat(fbdo, F("/User2/node1/Tds"), Tds1) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get Tds... %s\n", Firebase.getFloat(fbdo, F("/User2/node1/Tds")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());


    Serial.printf("Set Turbidity... %s\n", Firebase.setFloat(fbdo, F("/User2/node1/Turbidity"), Turbidity1) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get Turbidity... %s\n", Firebase.getFloat(fbdo, F("/User2/node1/Turbidity")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

    
    Serial.printf("Set Ph... %s\n", Firebase.setFloat(fbdo, F("/User2/node1/Ph"), Ph1) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get Ph... %s\n", Firebase.getFloat(fbdo, F("/User2/node1/Ph")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

   
    Serial.printf("Set Star... %s\n", Firebase.setFloat(fbdo, F("/User2/node1/Star"),Star1) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get Star... %s\n", Firebase.getFloat(fbdo, F("/User2/node1/Star")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

    
    Serial.printf("Set Tds... %s\n", Firebase.setFloat(fbdo, F("/User2/node2/Tds"), Tds2) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get Tds... %s\n", Firebase.getFloat(fbdo, F("/User2/node2/Tds")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());


    Serial.printf("Set Turbidity... %s\n", Firebase.setFloat(fbdo, F("/User2/node2/Turbidity"), Turbidity2) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get Turbidity... %s\n", Firebase.getFloat(fbdo, F("/User2/node2/Turbidity")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

    
    Serial.printf("Set Ph... %s\n", Firebase.setFloat(fbdo, F("/User2/node2/Ph"), Ph2) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get Ph... %s\n", Firebase.getFloat(fbdo, F("/User2/node2/Ph")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

   
    Serial.printf("Set Star... %s\n", Firebase.setFloat(fbdo, F("/User2/node2/Star"),Star2) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get Star... %s\n", Firebase.getFloat(fbdo, F("/User2/node2/Star")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

    delay(3000);
   // json.set(".sv","timestamp");
    json.set("time",getCurrentTimeString());
    
    Serial.printf("Set json... %s\n", Firebase.set(fbdo, F("/User2"), json) ? "ok" : fbdo.errorReason().c_str());



     
     Serial.printf("Set User3... %s\n", Firebase.setString(fbdo, F("/User3/Name"), usernames[2]) ? "ok" : fbdo.errorReason().c_str());

     Serial.printf("Get User3... %s\n", Firebase.getString(fbdo, F("/User3/Name")) ? String(fbdo.to<String>()).c_str() : fbdo.errorReason().c_str());


     Serial.printf("Set UserID3... %s\n", Firebase.setString(fbdo, F("/User3/UserId"), userIds[2]) ? "ok" : fbdo.errorReason().c_str());

     Serial.printf("Get UserID3... %s\n", Firebase.getString(fbdo, F("/User3/UserId")) ? String(fbdo.to<String>()).c_str() : fbdo.errorReason().c_str());


    Serial.printf("Set Tds... %s\n", Firebase.setFloat(fbdo, F("/User3/node1/Tds"), Tds1) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get Tds... %s\n", Firebase.getFloat(fbdo, F("/User3/node1/Tds")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());


    Serial.printf("Set Turbidity... %s\n", Firebase.setFloat(fbdo, F("/User3/node1/Turbidity"), Turbidity1) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get Turbidity... %s\n", Firebase.getFloat(fbdo, F("/User3/node1/Turbidity")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

    
    Serial.printf("Set Ph... %s\n", Firebase.setFloat(fbdo, F("/User3/node1/Ph"), Ph1) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get Ph... %s\n", Firebase.getFloat(fbdo, F("/User3/node1/Ph")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

   
    Serial.printf("Set Star... %s\n", Firebase.setFloat(fbdo, F("/User3/node1/Star"),Star1) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get Star... %s\n", Firebase.getFloat(fbdo, F("/User3/node1/Star")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

    
    Serial.printf("Set Tds... %s\n", Firebase.setFloat(fbdo, F("/User3/node2/Tds"), Tds2) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get Tds... %s\n", Firebase.getFloat(fbdo, F("/User3/node2/Tds")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());


    Serial.printf("Set Turbidity... %s\n", Firebase.setFloat(fbdo, F("/User3/node2/Turbidity"), Turbidity2) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get Turbidity... %s\n", Firebase.getFloat(fbdo, F("/User3/node2/Turbidity")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

    
    Serial.printf("Set Ph... %s\n", Firebase.setFloat(fbdo, F("/User3/node2/Ph"), Ph2) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get Ph... %s\n", Firebase.getFloat(fbdo, F("/User3/node2/Ph")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

   
    Serial.printf("Set Star... %s\n", Firebase.setFloat(fbdo, F("/User3/node2/Star"),Star2) ? "ok" : fbdo.errorReason().c_str());

    Serial.printf("Get Star... %s\n", Firebase.getFloat(fbdo, F("/User3/node2/Star")) ? String(fbdo.to<float>()).c_str() : fbdo.errorReason().c_str());

   
//    json.set("time", getCurrentTimeString());
    json.set("time",getCurrentTimeString());
    Serial.printf("Set json... %s\n", Firebase.set(fbdo, F("/User3"), json) ? "ok" : fbdo.errorReason().c_str());

    delay(3000); 
    
  Serial.println();
    count++;
  }

  
  
}

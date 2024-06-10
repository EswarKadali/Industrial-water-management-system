#include <EEPROM.h>
#include "GravityTDS.h"
#define TdsSensorPin A1
GravityTDS gravityTds;
#include <Wire.h>
//Arduino to NodeMCU Lib
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

//Initialise Arduino to NodeMCU (5=Rx & 6=Tx)
SoftwareSerial nodemcu(5, 6);


double x=0;
int buf[10];
float temp=0;
double flow; //Liters of passing water volume
unsigned long pulse_freq=0;
float avgValue=0; //Store the average value of the sensor feedback
float b,pherror=0,phrating=0;
float turbidityerror=0, turbidityrating=0, tdserror=0, tdsrating=0, waterrating=0;
float temperature = 25,tdsValue = 0;
unsigned long previousMillis = 0;
const long interval = 50000;


void setup()
{
Serial.begin(9600);
nodemcu.begin(9600);
gravityTds.setPin(TdsSensorPin);
gravityTds.setAref(5.0); //reference voltage on ADC, default 5.0V on Arduino UNO
gravityTds.setAdcRange(1024); //1024 for 10bit ADC;4096 for 12bit ADC
gravityTds.begin(); //initialization
pinMode(5, OUTPUT);
pinMode(2, INPUT);
attachInterrupt(0, pulse, RISING); // Setup Interrupt

}

void loop()
{
flow = .00225 * pulse_freq;
//Serial.print(flow, DEC);
// Serial.println("L");
//-----------------------pH-------------------------

for(int i=0;i<10;i++) //Get 10 sample value from the sensor for smooth the value
{
buf[i]=analogRead(A0);
delay(10);
}
for(int i=0;i<10;i++)
{
temp+=buf[i];
}
temp=(1000*temp/1024.0);
avgValue= (float)temp/200.0;
Serial.print("Ph value is:");
Serial.println(avgValue);

pherror=abs(7-avgValue)*10;
phrating=10-pherror;

//Serial.print("Ph rating is:");
//Serial.println(phrating);

for(int i=0;i<10;i++)
{
buf[i]=0;
temp=0;
}

//--------------------------TDS------------------


gravityTds.setTemperature(temperature); // set the temperature and execute temperature compensation
gravityTds.update(); //sample and calculate
tdsValue = gravityTds.getTdsValue(); // then get the value
Serial.print(tdsValue,0);
Serial.println("ppm");
delay(100);
tdserror= abs(100-tdsValue)/100;
tdsrating= 10-tdserror;
// Serial.print("TDS rating is:");
//Serial.println(tdsrating);

//---------------------Turbidity-----------------------

float turb = analogRead(A3);
float turbidity = turb * (5000.0 / 1024.0);
Serial.print("Turbidity is:");
Serial.println(turbidity);
turbidityerror= abs((400-turbidity)/20);
turbidityrating= 10-turbidityerror;

waterrating= (0.33*tdsrating) + (0.33*phrating) + (0.33*turbidityrating);

Serial.print("Water Rating is:");
Serial.println(waterrating);



StaticJsonBuffer<1000> jsonBuffer;
JsonObject& data = jsonBuffer.createObject();

//Assign collected data to JSON Object
data["Ph"] = avgValue;
data["Tds"] = tdsValue;
data["Turbidity"] = turbidity;
data["Flow"] = flow;

//Send data to NodeMCU
data.printTo(nodemcu);
jsonBuffer.clear();
delay(1500);
}

void pulse () // Interrupt function
{
pulse_freq++;
}

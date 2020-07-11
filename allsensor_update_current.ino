/*
  WriteMultipleFields in ThingSpeak
  
  Description: Writes values to fields 1,2,3,4,5,6,7,8 and status in a single ThingSpeak update every 60 seconds.
  
  Hardware: Arduino MKR WiFi 1010, Arduino MRK ENV, DS18B20.   
  
  !!! IMPORTANT - Modify the secrets.h file for this project with your network connection and ThingSpeak channel details. !!!
  
  Note:
  - Requires WiFiNINA library
  - This example is written for a network using WPA encryption. For WEP or WPA, change the WiFi.begin() call accordingly.
  
  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize, and 
  analyze live data streams in the cloud.
  
*/

#include "ThingSpeak.h"
#include <SPI.h>
#include <WiFiNINA.h>
#include "secrets.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Arduino_MKRENV.h>


Adafruit_INA219 ina219;


char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;


unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;


#define ONE_WIRE_BUS 0
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float Celsius_1 = 0;
float Celsius_2 = 0;


const int AirValue = 920;   //you need to replace this value with Value_1
const int WaterValue = 393;  //you need to replace this value with Value_2
int soilMoistureValue = 0;
int soilmoisturepercent=0;
float m;

// Initialize our values

String myStatus = "";

void setup() {
  Serial.begin(115200);  // Initialize serial

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv != "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }
  
  // Initialize Arduino MKRENV module
  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV shield!");
    while(1);
   }
  
  uint32_t currentFrequency;
  
  // Initialize INA2019 Temperature Sensor Module
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  ina219.setCalibration_16V_400mA();

  //Initialize ThingSpeak  
  ThingSpeak.begin(client);  
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
Serial.println(soilMoistureValue);
soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
if(soilmoisturepercent > 100)
{
  Serial.println("100 %");
  m=100;
}
else if(soilmoisturepercent <0)
{
  Serial.println("0 %");
  m=0;
}
else if(soilmoisturepercent >0 && soilmoisturepercent < 100)
{
  Serial.print(soilmoisturepercent);
  Serial.println("%");
  m=soilmoisturepercent;
  
}

//voltage Measurment
      float temp=0;
      for (int i=0;i<10;i++)
      {
      temp=temp+analogRead(A5);
      delay(100);
      }
      float A=temp/10;
      float v1 = A * (3.3/ 1023.0)*1000;
      Serial.println("v1");
      Serial.println(v1);
      Serial.println("mV");
      
      
      float temp2=0;
      for (int i=0;i<10;i++)
      {
      temp2=temp2+analogRead(A6);
      delay(100);
      }
      float B=temp2/10;
      float v2 = B * (3.3/ 1023.0)*1000;
      Serial.println("v2");
      Serial.println(v2);
      Serial.println("mV");
      
      float d_v=(v1-v2);
      Serial.println("d_v");
      Serial.println(d_v);
      Serial.println("mV");

// Humidity Measurment
      float temp3=0;
      for (int i=0;i<10;i++)
      {
      float hh = ENV.readHumidity();
      temp3=temp3+hh;
      delay(100);
      }
      float h=temp3/10;
      Serial.println("humd:");
      Serial.println(h);
      
// Temperature Measurment      
//      float temp4=0;
//      for (int i=0;i<10;i++)
//      {
//      float tt = ENV.readTemperature();
//      temp4=temp4+tt;
//      delay(100);
//      }
//      float t=temp4/10;
//      Serial.println("temp:");
//      Serial.println(t);


// Soil internal Temperature Measurment & Ambient Temperature Measurment    
    sensors.requestTemperatures();
    delay(1000);
    Celsius_1 = sensors.getTempCByIndex(0); // Soil internal Temperature Measurment
//  Fahrenheit = sensors.toFahrenheit(Celsius);
    Serial.print(Celsius_1);
    Serial.print(" C  ");
    delay(1000);
    
    Celsius_2 = sensors.getTempCByIndex(1); //Ambient Temperature Measurment 
    Serial.print(Celsius_2);
    Serial.print(" C  ");
//  Serial.print(Fahrenheit);
//  Serial.println(" F");
 
 //Measure Current from Soil Cell using INA219
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");

  delay(2000);

      

  // set the fields with the values
  ThingSpeak.setField(1, Celsius_2);
  ThingSpeak.setField(2, h);
  ThingSpeak.setField(3, m);
  ThingSpeak.setField(4, d_v);
  ThingSpeak.setField(5, Celsius_1);
  ThingSpeak.setField(6, current_mA);
  ThingSpeak.setField(7, loadvoltage);
  ThingSpeak.setField(8, power_mW);
  
  
  // set the status
  ThingSpeak.setStatus(myStatus);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  delay(60000); // Wait 20 seconds to update the channel again
}

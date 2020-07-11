# Environment-Monitoring-Station
Environment Monitoring Station for observing different characteristics of a Soil Cell.

## Description of the project
WriteMultipleFields in ThingSpeak. Writes values to fields 1,2,3,4,5,6,7,8 in a single ThingSpeak update every 60 seconds.

## Hardware: 
Arduino MKR WiFi 1010, Arduino MRK ENV, DS18B20, INA219.   
  
**!!! IMPORTANT - Modify the secrets.h file for this project with your network connection and ThingSpeak channel details. !!!**
  
**Note:**
- Requires WiFiNINA library
- This example is written for a network using WPA encryption. For WEP or WPA, change the WiFi.begin() call accordingly.
  
ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize, and 
analyze live data streams in the cloud.

Live Channel Link for the Monitoring Station https://thingspeak.com/channels/860812 

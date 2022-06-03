//This is a Relay based Electrical Apliance controller system with RTC clock and auto on off functionality
//Monish Meher (Catalyst)



//library Declerationss

#include <Wire.h>       //I2C library


#include <ArduinoJson.h> //JSON Parser Library

#include <NTPClient.h> //NTP Time Library


#include <WiFiUdp.h> //Required Networking Library


#include <RtcDS3231.h>  //RTC library

RtcDS3231<TwoWire> rtcObject(Wire); //needed by library v2.0.0


//----------------------------------------Main Code Starts----------------------------------------//

// Replace with your network credentials
const char *ssid     = "Vanadium";
const char *password = "meher123";


// Definining NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};



//defining pins
#define RELAY_PIN_1 D3   
#define RELAY_PIN_2 D4   
#define RELAY_PIN_3 D5   
#define RELAY_PIN_4 D6   




//setting up sinric pro
#define APP_KEY    "YOUR-APP-KEY"    // Should look like "de0bxxxx-1x3x-4x3x-ax2x-5dabxxxxxxxx"
#define APP_SECRET "YOUR-APP-SECRET" // Should look like "5f36xxxx-x3x7-4x3x-xexe-e86724a9xxxx-4c4axxxx-3x3x-x5xe-x9x3-333d65xxxxxx"

#define SWITCH_ID_1       "YOUR-DEVICE-ID"    // Should look like "5dc1564130xxxxxxxxxxxxxx"
#define SWITCH_ID_2       "YOUR-DEVICE-ID"    // Should look like "5dc1564130xxxxxxxxxxxxxx"
#define SWITCH_ID_3       "YOUR-DEVICE-ID"    // Should look like "5dc1564130xxxxxxxxxxxxxx"
#define SWITCH_ID_4       "YOUR-DEVICE-ID"    // Should look like "5dc1564130xxxxxxxxxxxxxx"


bool onPowerState1(const String &deviceId, bool &state) {
    digitalWrite(RELAY_PIN_1, state);  
    Serial.printf("Device 1 turned %s\r\n", state?"on":"off");
    return true; // request handled properly
}

bool onPowerState2(const String &deviceId, bool &state) {
    digitalWrite(RELAY_PIN_2, state);  
    Serial.printf("Device 2 turned %s\r\n", state?"on":"off");
    return true; // request handled properly
}

bool onPowerState3(const String &deviceId, bool &state) {
    digitalWrite(RELAY_PIN_3, state);   
    Serial.printf("Device 3 turned %s\r\n", state?"on":"off");
    return true; // request handled properly
}

bool onPowerState4(const String &deviceId, bool &state) {
    digitalWrite(RELAY_PIN_4, state);  
    Serial.printf("Device 4 turned %s\r\n", state?"on":"off");
    return true; // request handled properly
}





void setup() {
// Initialize Serial Monitor
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

   setupSinricPro();

// Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(19800);  //offset to get IST




    //get hardset values github as JSON

    //setting time as 7:00pm
    int hardSetTimeHour = "19" 
    int hardSetTimeMinute = "0"




    //For RTC 
    rtcObject.Begin();     //Starts I2C
    RtcDateTime currentTime = RtcDateTime(16, 05, 18, 21, 20, 0); //define date and time object
    rtcObject.SetDateTime(currentTime); //configure the RTC with object





}

void loop() {
  SinricPro.handle();
  //first get the current time CHOOSE 1
  //getTimeFromRTC()
  getTimeFromNTP()



  //broadcast/display the current time
    Serial.println(currentHour);    
    Serial.println(currentMinute); 





  //checking if channel off condition
    checkCondition()



}




void checkCondition(){

    if(currentHour == hardSetTimeHour && currentMinute == hardSetTimeMinute){

       switchOFF()

    }


}



void switchOFF(){
    onPowerState1 = false;
    onPowerState2 = false;
    onPowerState3 = false;
    onPowerState4 = false;

    digitalWrite(RELAY_PIN_1, LOW);  
    digitalWrite(RELAY_PIN_2, LOW);  
    digitalWrite(RELAY_PIN_3, LOW);  
    digitalWrite(RELAY_PIN_4, LOW);  

    mySwitch1.sendPowerStateEvent(onPowerState1); 
    mySwitch2.sendPowerStateEvent(onPowerState2); 
    mySwitch3.sendPowerStateEvent(onPowerState3); 
    mySwitch4.sendPowerStateEvent(onPowerState4); 

}






void getTimeFromNTP(){
 timeClient.update();

int currentHour = timeClient.getHours();
Serial.print("Hour: ");


int currentMinute = timeClient.getMinutes();
Serial.print("Minutes: ");

}




void getTimeFromRTC(){
  //function to get time from RTC module
   RtcDateTime currentTime = rtcObject.GetDateTime();   //get the time from the RTC
  char str[20];   //declare a string as an array of chars
  int currentHour = currentTime.getHours();
  int currentMinute = currentTime.getMinutes();
  Serial.println(str); 
  delay(20000);
}

void setupSinricPro() {
  // add devices and callbacks to SinricPro
  SinricProSwitch& mySwitch1 = SinricPro[SWITCH_ID_1];
  mySwitch1.onPowerState(onPowerState1);

  SinricProSwitch& mySwitch2 = SinricPro[SWITCH_ID_2];
  mySwitch2.onPowerState(onPowerState2);

  SinricProSwitch& mySwitch3 = SinricPro[SWITCH_ID_3];
  mySwitch3.onPowerState(onPowerState3);

  SinricProSwitch& mySwitch4 = SinricPro[SWITCH_ID_4];
  mySwitch4.onPowerState(onPowerState4);

  // setup SinricPro
  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); }); 
  SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}

//This is a Relay based Electrical Apliance controller system with RTC clock and auto on off functionality
//Monish Meher (Catalyst)



#include <Wire.h>       //I2C library


#include <ArduinoJson.h> //JSON Parser Library


#include <RtcDS3231.h>  //RTC library

RtcDS3231<TwoWire> rtcObject(Wire); //needed by library v2.0.0








void setup() {
    //Starts serial connection
    Serial.begin(115200);
  

    //get hardset values github as JSON













    //For RTC 
    rtcObject.Begin();     //Starts I2C
    RtcDateTime currentTime = RtcDateTime(16, 05, 18, 21, 20, 0); //define date and time object
    rtcObject.SetDateTime(currentTime); //configure the RTC with object





}

void loop() {

  //first get the current time
  getTimeFromRTC()
  //getTimeFromApi()
  //broadcast/display the current time




  //checking if current time == end time if(yes)=Turn Off All Channels




}




void getTimeFromRTC(){

  //function to get time from RTC module
   RtcDateTime currentTime = rtcObject.GetDateTime();   //get the time from the RTC
  
  char str[20];   //declare a string as an array of chars
  
  sprintf(str, "%d/%d/%d %d:%d:%d",     //%d allows to print an integer to the string
          currentTime.Year(),   //get year method
          currentTime.Month(),  //get month method
          currentTime.Day(),    //get day method
          currentTime.Hour(),   //get hour method
          currentTime.Minute(), //get minute method
          currentTime.Second()  //get second method
         );
  
  Serial.println(str); //print the string to the serial port
  
  delay(20000); //20 seconds delay


}


void getTimeFromAPI(){

const char* server = "https://timeapi.io/api/Time/current/zone?timeZone=Asia/Kolkata";
const unsigned long HTTP_TIMEOUT = 10000;  // max respone time from server
const size_t MAX_CONTENT_SIZE = 512;       // max size of the HTTP response
 
 
  //function to get time from RTC module


}



#include <FirebaseESP8266.h>
#include  <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>


#define ssid "Monish's iPhone"  //WiFi SSID
#define password "meh12345"  //WiFi Password
#define FIREBASE_HOST "smartlab-237b1-default-rtdb.firebaseio.com"       //Firebase Project URL Remove "https:" , "\" and "/"
#define FIREBASE_AUTH "Y3M76xsC35OVvwOqmUNWRmI0Dxngtnnft3FoQSMy"      //Firebase Auth Token

FirebaseData firebaseData;
FirebaseData fbdo;

int count = 0;

//If using Relay Module
int Device_1 = D1; //initialize D1 Pin
int Device_2 = D2; //initialize D2 Pin

//for Scheduling

int timeRTDB = 1900;
int timeEXT = 0;



//For NTP
const long utcOffsetInSeconds = 19800;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);




void setup() {
  
 Serial.begin(9600);
   WiFi.begin (ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println ("");
  Serial.println ("WiFi Connected!");
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  
    pinMode(Device_1,OUTPUT);//initialize the Device OUTPUT  
    pinMode(Device_2,OUTPUT);//initialize the Device OUTPUT 
     
  digitalWrite(Device_1,HIGH);
  digitalWrite(Device_2,HIGH);

  timeClient.begin();
}




void loop() {

  //check scheduled time first

  checkHardOFF();
  loopStatusCheck();
  checkForExtention();


  }






  void loopStatusCheck(){
    
//  for fan

  if (Firebase.get(firebaseData,"Devices/Fan/1")) {
    if (firebaseData.dataType() == "boolean") {
      bool fan = firebaseData.boolData();
          if (fan==true){
              digitalWrite(Device_1,HIGH); //Device1 is ON
               Serial.println ("Fan ON");
          }
          else if (fan==false){
              digitalWrite(Device_1,LOW);//Device1 if OFF
                  Serial.println ("Fan OFF");
            }}}




   
//  for light

  if (Firebase.get(firebaseData,"Devices/Light/1")) {
    if (firebaseData.dataType() == "boolean") {
      bool light = firebaseData.boolData();
          if (light==true){
              digitalWrite(Device_2,HIGH); //Device1 is ON
               Serial.println ("LIGHT ON");
          }
          else if (light==false){
              digitalWrite(Device_2,LOW);//Device1 if OFF
                  Serial.println ("LIGHT OFF");
            }}}
    }




void checkHardOFF(){

    getTimeFromRTDB();
    timeClient.update();

        Serial.print("Databse Time");
        Serial.println(timeRTDB);

    int timeCurrentHours = (timeClient.getHours()*100)-timeEXT;
    int timeCurrentMinutes = timeClient.getMinutes();

    int timeCurrentFinal = timeCurrentHours+timeCurrentMinutes;
      Serial.print("Current Time");
        Serial.println(timeCurrentFinal);

    int timeDiff = (timeRTDB - timeCurrentFinal);
      Serial.print("Difference");
    Serial.println(timeDiff);
    

    if(timeDiff <= 0){
        digitalWrite(Device_2,LOW); 
        Serial.println ("LIGHT OFF");
        Firebase.RTDB.setBool(&fbdo, "Devices/Light/1", false);
               
        digitalWrite(Device_1,LOW);
        Serial.println ("FAN OFF");
         Firebase.RTDB.setBool(&fbdo, "Devices/Fan/1", false);
          timeEXT = 0;
         checkForExtention();
        
      }
      
  
  
  }




void getTimeFromRTDB(){
   if (Firebase.get(firebaseData,"Schedule/offTimeSet")) {
      timeRTDB = firebaseData.intData();
    }
   }




void checkForExtention(){
  if (Firebase.get(firebaseData,"Extention/usr")) {
    if (firebaseData.dataType() == "boolean") {
      bool ext = firebaseData.boolData();
          if (ext==true){
             Serial.println ("time extended");
            turnONAll();
           
            

          }
    }
  }
}

void turnONAll(){

        digitalWrite(Device_2,HIGH); 
        Serial.println ("LIGHT ON");
        Firebase.RTDB.setBool(&fbdo, "Devices/Light/1", true);
               
        digitalWrite(Device_1,HIGH);
        Serial.println ("FAN ON");
         Firebase.RTDB.setBool(&fbdo, "Devices/Fan/1", true);

          timeEXT  = 100;
            Firebase.RTDB.setBool(&fbdo, "Extention/usr", false);
         
         }
    

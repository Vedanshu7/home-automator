#include "SoftwareSerial.h"
SoftwareSerial esp8266(12, 13); // RX, TX
#include <Wire.h>
#include "Sodaq_DS3231.h"
char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
int Hour1,Min,Sec,Hour2,Min1,Sec1;
String On,Off;
#define a1 A0
#define b1 A1
#define c1 A2
#define d1 A3
void setup()
{
  pinMode(a1, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(c1, OUTPUT);
  pinMode(d1, OUTPUT);
  digitalWrite(a1,HIGH);
  digitalWrite(b1,HIGH);
  digitalWrite(c1,HIGH);
  digitalWrite(d1,HIGH);
  Serial.begin(9600);
  esp8266.begin(115200);  // your ESP's baud rate might be different
  esp8266.println("AT+CWMODE=2");
  delay(200);
  esp8266.println("AT+CIPMUX=1");
  delay(200);
  esp8266.println("AT+CIPSERVER=1,333");
   Wire.begin();
  rtc.begin();
}
uint32_t old_ts;
void loop()
{
  delay(10);
  String str = "", Ac = "";
   DateTime now = rtc.now(); //get the current date-time
    uint32_t ts = now.getEpoch();
  if (esp8266.available()) // check if the ESP is sending a message
  {
    
    while (esp8266.available())
    {
      str += (char) esp8266.read();
    }
    String A = manageString(str);
    Serial.println(A);
    if (A.length() == 2) {
     
      if (A == "11") {
        digitalWrite(a1, HIGH);
      }
      else if (A == "55") {
        digitalWrite(a1, LOW);
      }
      else if (A == "15" ) {
        digitalWrite(b1, HIGH);
      }
      else if (A == "60") {
        digitalWrite(b1,LOW);
      }
      else if (A == "44") {
        digitalWrite(c1, HIGH);
      }
      else if (A == "77") {
        digitalWrite(c1, LOW);
        
      }
     else if (A == "95") {
        digitalWrite(d1, HIGH);
        
      }
      else if (A == "88") {
        digitalWrite(d1, LOW);
        //delay(1000);
      }
      
    }
    else if(A.length()==16){
         passTheString(A);
    }
  }
  if (old_ts == 0 || old_ts != ts) {
      old_ts = ts;
      DateTime now = rtc.now();
      int h11,m11,s11;
      h11=now.hour();
      m11=now.minute();
      s11=now.second();
    if(h11 == Hour1 && m11==Min && s11==Sec){
      if(On.length()==1){
         if(On=="1"){
          digitalWrite(a1,LOW);
         }
         else if(On=="2"){
          digitalWrite(b1,LOW);
         }
         else if(On=="3"){
          digitalWrite(c1,LOW);
         }
         else if(On=="4"){
          digitalWrite(d1,LOW);
         }
      }
      else if(On.length()==2){
        if(On=="12"){
          digitalWrite(a1,LOW);
          digitalWrite(b1,LOW);
         }
         else if(On=="13"){
          digitalWrite(a1,LOW);
          digitalWrite(c1,LOW);
         }
         else if(On=="14"){
          digitalWrite(a1,LOW);
          digitalWrite(d1,LOW);
         }
         else if(On=="23"){
          digitalWrite(b1,LOW);
          digitalWrite(c1,LOW);
         }
         else if(On=="24"){
          digitalWrite(b1,LOW);
          digitalWrite(d1,LOW);
         }
         else if(On=="34"){
          digitalWrite(c1,LOW);
          digitalWrite(d1,LOW);
         }
      }
      else if(On.length()==3){
        if(On=="123"){
          digitalWrite(a1,LOW);
          digitalWrite(b1,LOW);
          digitalWrite(c1,LOW);
         }
         else if(On=="134"){
          digitalWrite(a1,LOW);
          digitalWrite(c1,LOW);
          digitalWrite(d1,LOW);
         }
         else if(On=="234"){
          digitalWrite(b1,LOW);
          digitalWrite(c1,LOW);
          digitalWrite(d1,LOW);
         }
         else if(On=="124"){
          digitalWrite(a1,LOW);
          digitalWrite(b1,LOW);
          digitalWrite(d1,LOW);
         }
      }
      else if(On.length()==4){
        if(On=="1234"){
          digitalWrite(a1,LOW);
          digitalWrite(b1,LOW);
          digitalWrite(c1,LOW);
          digitalWrite(d1,LOW);
        }
      }
    }
    if(h11 == Hour2 && m11==Min1 && s11==Sec1){
      if(Off.length()==1){
         if(Off=="1"){
          digitalWrite(a1,HIGH);
         }
         else if(Off=="2"){
          digitalWrite(b1,HIGH);
         }
         else if(Off=="3"){
          digitalWrite(c1,HIGH);
         }
         else if(Off=="4"){
          digitalWrite(d1,HIGH);
         }
      }
      else if(Off.length()==2){
        if(Off=="12"){
          digitalWrite(a1,HIGH);
          digitalWrite(b1,HIGH);
         }
         else if(Off=="13"){
          digitalWrite(a1,HIGH);
          digitalWrite(c1,HIGH);
         }
         else if(Off=="14"){
          digitalWrite(a1,HIGH);
          digitalWrite(d1,HIGH);
         }
         else if(Off=="23"){
          digitalWrite(b1,HIGH);
          digitalWrite(c1,HIGH);
         }
         else if(Off=="24"){
          digitalWrite(b1,HIGH);
          digitalWrite(d1,HIGH);
         }
         else if(Off=="34"){
          digitalWrite(c1,HIGH);
          digitalWrite(d1,HIGH);
         }
      }
      else if(Off.length()==3){
        if(Off=="123"){
          digitalWrite(a1,HIGH);
          digitalWrite(b1,HIGH);
          digitalWrite(c1,HIGH);
         }
         else if(Off=="134"){
          digitalWrite(a1,HIGH);
          digitalWrite(c1,HIGH);
          digitalWrite(d1,HIGH);
         }
         else if(Off=="234"){
          digitalWrite(b1,HIGH);
          digitalWrite(c1,HIGH);
          digitalWrite(d1,HIGH);
         }
         else if(Off=="124"){
         digitalWrite(a1,HIGH);
          digitalWrite(b1,HIGH);
          digitalWrite(d1,HIGH);
         }
      }
      else if(Off.length()==4){
        if(Off=="1234"){
          digitalWrite(a1,HIGH);
          digitalWrite(b1,HIGH);
          digitalWrite(c1,HIGH);
          digitalWrite(d1,HIGH);
        }
      }
    }
  }
    if (Serial.available())
    {
      delay(10);  // wait to let all the input command in the serial buffer

      // read the input command in a string
      String cmd = "";
      while (Serial.available())
      {
        cmd += (char)Serial.read();
      }
      // send to the esp8266
      esp8266.println(cmd);
    }
    
  }
  String manageString(String str) {
    String Ac = "";
    for (int i = 0; str[i] != '\0'; i++) {
      if (str[i] >= '0' && str[i] <= '9') {
        Ac += str[i];
      }
    }
    
    if (Ac.length() == 5) {
      Ac = Ac.substring(2, 4);
     // Serial.print(Ac);
    }
    else if (Ac.length() == 20) {
      Ac = Ac.substring(3, 19);
      //Serial.println(Ac);
    }
    else
    {
      if(Ac.length()==16){
      }
      else{
        Ac="0";
      }
    }
    return Ac;
  }
  void passTheString(String S){
    String b=S.substring(10,11);
    String c=S.substring(12,13);
    String d=S.substring(0,4);
    String f=S.substring(4,6);
    String g=S.substring(6,8);
    String o=S.substring(14,15);
    String h=S.substring(15,16);
    int hou,mint,sec;
    DateTime now = rtc.now();
    hou=now.hour();
    mint=now.minute();
    sec=now.second();
    Serial.println(hou);
    Serial.println(mint);
    Serial.println(sec);
    int q=f.toInt();
    int p=g.toInt();
    if(o=="6" || h=="6"){
    if(b=="2"){
      if(c=="1"){
        if(d=="1000"){
          if(q-hou==2){
            Hour1=hou+2;
            Min=mint;
            Sec=sec;
            On="1";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="0200"){
          if(q-hou==2){
            Hour1=hou+2;
            Min=mint;
            Sec=sec;
            On="2";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="0030"){
          if(q-hou==2){
            Hour1=hou+2;
            Min=mint;
            Sec=sec;
            On="3";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="0004"){
          if(q-hou==2){
            Hour1=hou+2;
            Min=mint;
            Sec=sec;
            On="4";
            digitalWrite(a1,LOW);
          }
        }
      }
      else if(c=="2"){
        if(d=="1200"){
          if(q-hou==2){
            Hour1=hou+2;
            Min=mint;
            Sec=sec;
            On="12";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="1030"){
          if(q-hou==2){
            Hour1=hou+2;
            Min=mint;
            Sec=sec;
            On="13";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="1004"){
          if(q-hou==2){
            Hour1=hou+2;
            Min=mint;
            Sec=sec;
            On="14";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="0230"){
          if(q-hou==2){
            Hour1=hou+2;
            Min=mint;
            Sec=sec;
            On="23";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="0204"){
          if(q-hou==2){
            Hour1=hou+2;
            Min=mint;
            Sec=sec;
            On="24";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="0034"){
          if(q-hou==2){
            Hour1=hou+2;
            Min=mint;
            Sec=sec;
            On="34";
            digitalWrite(a1,LOW);
          }
        }
      }
      else if(c=="3"){
        if(d=="1230"){
          if(q-hou==2){
            Hour1=hou+2;
            Min=mint;
            Sec=sec;
            On="123";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="0234"){
          if(q-hou==2){
            Hour1=hou+2;
            Min=mint;
            Sec=sec;
            On="234";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="1034"){
          if(q-hou==2){
            Hour1=hou+2;
            Min=mint;
            Sec=sec;
            On="134";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="1204"){
          if(q-hou==2){
            Hour1=hou+2;
            Min=mint;
            Sec=sec;
            On="124";
            digitalWrite(a1,LOW);
          }
        }
      }
      else if(c=="4"){
        if(d=="1234"){
         if(q-hou==2){
            Hour1=hou+2;
            Min=mint;
            Sec=sec;
            On="1234";
            digitalWrite(a1,LOW);
          }
        }
      }
    }
    else if(b=="3"){
         if(c=="1"){
        if(d=="1000"){
          if(q-hou==3){
            Hour1=hou+3;
            Min=mint;
            Sec=sec;
            On="1";
            digitalWrite(a1,LOW);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour1=hou;
            Min=mint+2;
            Sec=sec;
            On="1";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="0200"){
          if(q-hou==3){
            Hour1=hou+3;
            Min=mint;
            Sec=sec;
            On="2";
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour1=hou;
            Min=mint+2;
            Sec=sec;
            On="2";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="0030"){
          if(q-hou==3){
            Hour1=hou+3;
            Min=mint;
            Sec=sec;
            On="3";
            digitalWrite(a1,LOW);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour1=hou;
            Min=mint+2;
            Sec=sec;
            On="3";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="0004"){
          if(q-hou==3){
            Hour1=hou+3;
            Min=mint;
            Sec=sec;
            On="4";
            digitalWrite(a1,LOW);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour1=hou;
            Min=mint+2;
            Sec=sec;
            On="4";
            digitalWrite(a1,LOW);
          }
        }
      }
      else if(c=="2"){
        if(d=="1200"){
          if(q-hou==3){
            Hour1=hou+3;
            Min=mint;
            Sec=sec;
            On="12";
            digitalWrite(a1,LOW);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour1=hou;
            Min=mint+2;
            Sec=sec;
            On="12";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="1030"){
          if(q-hou==3){
            Hour1=hou+3;
            Min=mint;
            Sec=sec;
            On="13";
            digitalWrite(a1,LOW);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour1=hou;
            Min=mint+2;
            Sec=sec;
            On="13";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="1004"){
          if(q-hou==3){
            Hour1=hou+3;
            Min=mint;
            Sec=sec;
            On="14";
            digitalWrite(a1,LOW);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour1=hou;
            Min=mint+2;
            Sec=sec;
            On="14";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="0230"){
          if(q-hou==3){
            Hour1=hou+3;
            Min=mint;
            Sec=sec;
            On="23";
            digitalWrite(a1,LOW);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour1=hou;
            Min=mint+2;
            Sec=sec;
            On="23";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="0204"){
          if(q-hou==3){
            Hour1=hou+3;
            Min=mint;
            Sec=sec;
            On="24";
            digitalWrite(a1,LOW);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour1=hou;
            Min=mint+2;
            Sec=sec;
            On="24";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="0034"){
          if(q-hou==3){
            Hour1=hou+3;
            Min=mint;
            Sec=sec;
            On="34";
            digitalWrite(a1,LOW);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour1=hou;
            Min=mint+2;
            Sec=sec;
            On="34";
            digitalWrite(a1,LOW);
          }
        }
      }
      else if(c=="3"){
        if(d=="1230"){
          if(q-hou==3){
            Hour1=hou+3;
            Min=mint;
            Sec=sec;
            On="123";
            digitalWrite(a1,LOW);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour1=hou;
            Min=mint+2;
            Sec=sec;
            On="123";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="0234"){
          if(q-hou==3){
            Hour1=hou+3;
            Min=mint;
            Sec=sec;
            On="234";
            digitalWrite(a1,LOW);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour1=hou;
            Min=mint+2;
            Sec=sec;
            On="234";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="1034"){
          if(q-hou==3){
            Hour1=hou+3;
            Min=mint;
            Sec=sec;
            On="134";
            digitalWrite(a1,LOW);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour1=hou;
            Min=mint+2;
            Sec=sec;
            On="134";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="1204"){
          if(q-hou==3){
            Hour1=hou+3;
            Min=mint;
            Sec=sec;
            On="124";
            digitalWrite(a1,LOW);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour1=hou;
            Min=mint;
            Sec=sec;
            On="124";
            digitalWrite(a1,LOW);
          }
        }
      }
      else if(c=="4"){
        if(d=="1234"){
         if(q-hou==3){
            Hour1=hou+3;
            Min=mint;
            Sec=sec;
            On="1234";
            digitalWrite(a1,LOW);
          }
          else if(q==hou && p-mint==2){
            Hour1=hou;
            Min=mint+2;
            Sec=sec;
            On="1234";
            digitalWrite(a1,LOW);
            Serial.println("Sated");
          }
        }
      }
    }
    else if(b=="4"){
      if(c=="1"){
        if(d=="1000"){
          if(q-hou==4){
            Hour1=hou+4;
            Min=mint;
            Sec=sec;
            On="1";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="0200"){
          if(q-hou==4){
            Hour1=hou+4;
            Min=mint;
            Sec=sec;
            On="2";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="0030"){
          if(q-hou==4){
            Hour1=hou+4;
            Min=mint;
            Sec=sec;
            On="3";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="0004"){
          if(q-hou==4){
            Hour1=hou+4;
            Min=mint;
            Sec=sec;
            On="4";
            digitalWrite(a1,LOW);
          }
        }
      }
      else if(c=="2"){
        if(d=="1200"){
          if(q-hou==4){
           Hour1=hou+4;
            Min=mint;
            Sec=sec;
            On="12";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="1030"){
          if(q-hou==4){
            Hour1=hou+4;
            Min=mint;
            Sec=sec;
            On="13";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="1004"){
          if(q-hou==4){
            Hour1=hou+4;
            Min=mint;
            Sec=sec;
            On="14";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="0230"){
          if(q-hou==4){
            Hour1=hou+4;
            Min=mint;
            Sec=sec;
            On="23";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="0204"){
          if(q-hou==4){
            Hour1=hou+4;
            Min=mint;
            Sec=sec;
            On="24";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="0034"){
          if(q-hou==4){
            Hour1=hou+4;
            Min=mint;
            Sec=sec;
            On="34";
            digitalWrite(a1,LOW);
          }
        }
      }
      else if(c=="3"){
        if(d=="1230"){
          if(q-hou==4){
            Hour1=hou+4;
            Min=mint;
            Sec=sec;
            On="123";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="0234"){
          if(q-hou==4){
           Hour1=hou+4;
            Min=mint;
            Sec=sec;
            On="234";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="1034"){
          if(q-hou==4){
            Hour1=hou+4;
            Min=mint;
            Sec=sec;
            On="134";
            digitalWrite(a1,LOW);
          }
        }
        else if(d=="1204"){
          if(q-hou==4){
            Hour1=hou+4;
            Min=mint;
            Sec=sec;
            On="124";
            digitalWrite(a1,LOW);
          }
        }
      }
      else if(c=="4"){
        if(d=="1234"){
          if(q-hou==4){
            Hour1=hou+4;
            Min=mint;
            Sec=sec;
            On="1234";
            digitalWrite(a1,LOW);
          }
        }
      }
    }
    else{
      
    }
   }
   else if(o=="9" || h=="9"){
    if(b=="2"){
      if(c=="1"){
        if(d=="1000"){
          if(q-hou==2){
            Hour2=hou+2;
            Min1=mint;
            Sec1=sec;
            Off="1";
            digitalWrite(b1,HIGH);
          }
        }
        else if(d=="0200"){
          if(q-hou==2){
            Hour2=hou+2;
            Min1=mint;
            Sec1=sec;
            Off="2";
            digitalWrite(b1,HIGH);
          }
        }
        else if(d=="0030"){
          if(q-hou==2){
            Hour2=hou+2;
            Min1=mint;
            Sec1=sec;
            Off="3";
            digitalWrite(b1,HIGH);
          }
        }
        else if(d=="0004"){
          if(q-hou==2){
            Hour2=hou+2;
            Min1=mint;
            Sec1=sec;
            Off="4";
            digitalWrite(b1,HIGH);
          }
        }
      }
      else if(c=="2"){
        if(d=="1200"){
          if(q-hou==2){
            Hour2=hou+2;
            Min1=mint;
            Sec1=sec;
            Off="12";
            digitalWrite(b1,HIGH);
          }
        }
        else if(d=="1030"){
          if(q-hou==2){
            Hour2=hou+2;
            Min1=mint;
            Sec1=sec;
            Off="13";
            digitalWrite(b1,HIGH);
          }
        }
        else if(d=="1004"){
          if(q-hou==2){
            Hour2=hou+2;
            Min1=mint;
            Sec1=sec;
            Off="14";
            digitalWrite(b1,HIGH);
          }
        }
        else if(d=="0230"){
          if(q-hou==2){
            Hour2=hou+2;
            Min1=mint;
            Sec1=sec;
            Off="23";
            digitalWrite(b1,HIGH);
          }
        }
        else if(d=="0204"){
          if(q-hou==2){
            Hour2=hou+2;
            Min1=mint;
            Sec1=sec;
            Off="24";
            digitalWrite(b1,HIGH);
          }
        }
        else if(d=="0034"){
          if(q-hou==2){
            Hour2=hou+2;
            Min1=mint;
            Sec1=sec;
            Off="34";
           digitalWrite(b1,HIGH);
          }
        }
      }
      else if(c=="3"){
        if(d=="1230"){
          if(q-hou==2){
            Hour2=hou+2;
            Min1=mint;
            Sec1=sec;
            Off="123";
            digitalWrite(b1,HIGH);
          }
        }
        else if(d=="0234"){
          if(q-hou==2){
            Hour2=hou+2;
            Min1=mint;
            Sec1=sec;
           digitalWrite(b1,HIGH);
          }
        }
        else if(d=="1034"){
          if(q-hou==2){
            Hour2=hou+2;
            Min1=mint;
            Sec1=sec;
            Off="134";
            digitalWrite(b1,HIGH);
          }
        }
        else if(d=="1204"){
          if(q-hou==2){
            Hour2=hou+2;
            Min1=mint;
            Sec1=sec;
            Off="124";
            digitalWrite(b1,HIGH);
          }
        }
      }
      else if(c=="4"){
        if(d=="1234"){
         if(q-hou==2){
           Hour2=hou+2;
            Min1=mint;
            Sec1=sec;
            Off="1234";
            digitalWrite(b1,HIGH);
          }
        }
      }
    }
    else if(b=="3"){
         if(c=="1"){
        if(d=="1000"){
          if(q-hou==3){
            Hour2=hou+3;
            Min1=mint;
            Sec1=sec;
            Off="1";
           digitalWrite(b1,HIGH);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour2=hou;
            Min1=mint+2;
            Sec1=sec;
            Off="1";
            digitalWrite(b1,HIGH);
          }
        }
        else if(d=="0200"){
          if(q-hou==3){
            Hour2=hou+3;
            Min1=mint;
            Sec1=sec;
            Off="2";
            digitalWrite(b1,HIGH);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour2=hou;
            Min1=mint+2;
            Sec1=sec;
            Off="2";
            digitalWrite(b1,HIGH);
          }
        }
        else if(d=="0030"){
          if(q-hou==3){
            Hour2=hou+3;
            Min1=mint;
            Sec1=sec;
            Off="3";
            digitalWrite(b1,HIGH);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour2=hou;
            Min1=mint+2;
            Sec1=sec;
            Off="3";
           digitalWrite(b1,HIGH);
          }
        }
        else if(d=="0004"){
          if(q-hou==3){
            Hour2=hou+3;
            Min1=mint;
            Sec1=sec;
            Off="4";
            digitalWrite(b1,HIGH);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour2=hou;
            Min1=mint+2;
            Sec1=sec;
            Off="4";
            digitalWrite(b1,HIGH);
          }
        }
      }
      else if(c=="2"){
        if(d=="1200"){
          if(q-hou==3){
            Hour2=hou+3;
            Min1=mint;
            Sec1=sec;
            Off="12";
            digitalWrite(b1,HIGH);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour2=hou;
            Min1=mint+2;
            Sec1=sec;
            Off="12";
           digitalWrite(b1,HIGH);
          }
        }
        else if(d=="1030"){
          if(q-hou==3){
            Hour2=hou+3;
            Min1=mint;
            Sec1=sec;
            Off="13";
            digitalWrite(b1,HIGH);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour2=hou;
            Min1=mint+2;
            Sec1=sec;
            Off="13";
           digitalWrite(b1,HIGH);
          }
        }
        else if(d=="1004"){
          if(q-hou==3){
            Hour2=hou+3;
            Min1=mint;
            Sec1=sec;
            Off="14";
           digitalWrite(b1,HIGH);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour2=hou;
            Min1=mint+2;
            Sec1=sec;
            Off="14";
           digitalWrite(b1,HIGH);
          }
        }
        else if(d=="0230"){
          if(q-hou==3){
            Hour2=hou+3;
            Min1=mint;
            Sec1=sec;
            Off="23";
            digitalWrite(b1,HIGH);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour2=hou;
            Min1=mint+2;
            Sec1=sec;
            Off="23";
           digitalWrite(b1,HIGH);
          }
        }
        else if(d=="0204"){
          if(q-hou==3){
            Hour2=hou+3;
            Min1=mint;
            Sec1=sec;
            Off="24";
           digitalWrite(b1,HIGH);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour2=hou;
            Min1=mint+2;
            Sec1=sec;
            Off="24";
           digitalWrite(b1,HIGH);
          }
        }
        else if(d=="0034"){
          if(q-hou==3){
            Hour2=hou+3;
            Min1=mint;
            Sec1=sec;
            Off="34";
           digitalWrite(b1,HIGH);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour2=hou;
            Min1=mint+2;
            Sec1=sec;
            Off="34";
          digitalWrite(b1,HIGH);
          }
        }
      }
      else if(c=="3"){
        if(d=="1230"){
          if(q-hou==3){
            Hour2=hou+3;
            Min1=mint;
            Sec1=sec;
            Off="123";
           digitalWrite(b1,HIGH);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour2=hou;
            Min1=mint+2;
            Sec1=sec;
            Off="123";
           digitalWrite(b1,HIGH);
          }
        }
        else if(d=="0234"){
          if(q-hou==3){
            Hour2=hou+3;
            Min1=mint;
            Sec1=sec;
            Off="234";
            digitalWrite(b1,HIGH);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour2=hou;
            Min1=mint+2;
            Sec1=sec;
            Off="234";
           digitalWrite(b1,HIGH);
          }
        }
        else if(d=="1034"){
          if(q-hou==3){
            Hour2=hou+3;
            Min1=mint;
            Sec1=sec;
            Off="134";
           digitalWrite(b1,HIGH);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
           Hour2=hou;
            Min1=mint+2;
            Sec1=sec;
            Off="134";
           digitalWrite(b1,HIGH);
          }
        }
        else if(d=="1204"){
          if(q-hou==3){
            Hour2=hou+3;
            Min1=mint;
            Sec1=sec;
            Off="124";
          digitalWrite(b1,HIGH);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour2=hou;
            Min1=mint+2;
            Sec1=sec;
            Off="124";
          digitalWrite(b1,HIGH);
          }
        }
      }
      else if(c=="4"){
        if(d=="1234"){
         if(q-hou==3){
            Hour2=hou+3;
            Min1=mint;
            Sec1=sec;
            Off="1234";
            digitalWrite(b1,HIGH);
          }
          else if(q==hou && p-mint==2 || p-mint==3){
            Hour2=hou;
            Min1=mint+2;
            Sec1=sec;
            Off="1234";
            digitalWrite(b1,HIGH);
            Serial.println("Sated");
          }
        }
      }
    }
    else if(b=="4"){
      if(c=="1"){
        if(d=="1000"){
          if(q-hou==4){
            Hour2=hou+4;
            Min1=mint;
            Sec1=sec;
            Off="1";
            digitalWrite(b1,HIGH);
          }
        }
        else if(d=="0200"){
          if(q-hou==4){
            Hour2=hou+4;
            Min1=mint;
            Sec1=sec;
            Off="2";
            digitalWrite(b1,HIGH);
          }
        }
        else if(d=="0030"){
          if(q-hou==4){
            Hour2=hou+4;
            Min1=mint;
            Sec1=sec;
            Off="3";
           digitalWrite(b1,HIGH);
          }
        }
        else if(d=="0004"){
          if(q-hou==4){
            Hour2=hou+4;
            Min1=mint;
            Sec1=sec;
            Off="4";
            digitalWrite(b1,HIGH);
          }
        }
      }
      else if(c=="2"){
        if(d=="1200"){
          if(q-hou==4){
           Hour2=hou+4;
            Min1=mint;
            Sec1=sec;
            Off="12";
            digitalWrite(b1,HIGH);
          }
        }
        else if(d=="1030"){
          if(q-hou==4){
            Hour2=hou+4;
            Min1=mint;
            Sec1=sec;
            Off="13";
            digitalWrite(b1,HIGH);
          }
        }
        else if(d=="1004"){
          if(q-hou==4){
           Hour2=hou+4;
            Min1=mint;
            Sec1=sec;
            Off="14";
            digitalWrite(b1,HIGH);
          }
        }
        else if(d=="0230"){
          if(q-hou==4){
            Hour2=hou+4;
            Min1=mint;
            Sec1=sec;
            Off="23";
           digitalWrite(b1,HIGH);
          }
        }
        else if(d=="0204"){
          if(q-hou==4){
            Hour2=hou+4;
            Min1=mint;
            Sec1=sec;
            Off="24";
            digitalWrite(b1,HIGH);
          }
        }
        else if(d=="0034"){
          if(q-hou==4){
            Hour2=hou+4;
            Min1=mint;
            Sec1=sec;
            Off="34";
            digitalWrite(b1,HIGH);
          }
        }
      }
      else if(c=="3"){
        if(d=="1230"){
          if(q-hou==4){
            Hour2=hou+4;
            Min1=mint;
            Sec1=sec;
            Off="123";
            digitalWrite(b1,HIGH);
          }
        }
        else if(d=="0234"){
          if(q-hou==4){
           Hour2=hou+4;
            Min1=mint;
            Sec1=sec;
            Off="234";
            digitalWrite(b1,HIGH);
          }
        }
        else if(d=="1034"){
          if(q-hou==4){
            Hour2=hou+4;
            Min1=mint;
            Sec1=sec;
            Off="134";
            digitalWrite(b1,HIGH);
          }
        }
        else if(d=="1204"){
          if(q-hou==4){
           Hour2=hou+4;
            Min1=mint;
            Sec1=sec;
            Off="124";
            digitalWrite(b1,HIGH);
          }
        }
      }
      else if(c=="4"){
        if(d=="1234"){
          if(q-hou==4){
            Hour2=hou+4;
            Min1=mint;
            Sec1=sec;
            Off="1234";
            digitalWrite(b1,HIGH);
          }
        }
      }
    }
    else{
      
    }
   }
  }
 

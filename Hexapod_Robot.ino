#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

#define SERVOMIN  100
#define SERVOMAX  500
#define delay1 100
#define naik_turun 100
#define TOLERANSI 5
#define BLYNK_PRINT Serial

char auth[] = "35ab4985dcb946c7a0910091a8068a5d";
char ssid[] = "ASUS_X01BDA";
char pass[] = "19960607";
float servo;
int v0, v1, v2, v4;
int langkah = 0;
float heading, atas, bawah, posisi, setpoint;

void baca_kompas(){
  sensors_event_t event; 
  mag.getEvent(&event);
  heading = atan2(event.magnetic.y, event.magnetic.x)*180/PI + 180;
}
void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pwm.begin();
  pwm.setPWMFreq(50);
  Wire.begin();
  mag.begin();
  baca_kompas();
  setpoint = heading;
  delay(2000);
}

void loop() {
  baca_kompas();
  atas = setpoint + TOLERANSI;
  if (atas > 360) atas -= 360;
  bawah = setpoint - TOLERANSI;
  if (bawah < 0) bawah += 360;
  posisi = heading;
  if (posisi > atas) belok(5);
  if (posisi < bawah) belok(-5);

  Blynk.run();
  Blynk.virtualWrite(V5, langkah);
  if (v0 == 1) {
    if (v1 == 1) {
      if (v2 == 1){
        belok(10);
        setpoint = heading;
        langkah += 2;
      } else if (v4 == 1) {
        belok(-10);
        setpoint = heading;
        langkah += 2;
      }
      maju();
    }
    else mundur();
    langkah += 2;
  }
  else {
    berhenti();
    langkah = 0;
  }
//  Serial.print(langkah);
  Serial.println();
}

BLYNK_WRITE(V1) { //maju
  v1 = param.asInt();
}
BLYNK_WRITE(V0) { //mundur
  v0 = param.asInt();
}
BLYNK_WRITE(V2){ //kanan
  v2 = param.asInt();
}
BLYNK_WRITE(V4){ //kanan
  v4 = param.asInt();
}
BLYNK_WRITE(V3){
  int v3 = param.asInt();
  if (v3 == 1) {
    for (int i = 0; i < 4; i++){
      putar_balik();
    }
    setpoint += 180;
    if (setpoint > 360) setpoint -= 360;
  }
  else berhenti();
}
//BLYNK_WRITE(V5){
//  Blynk.virtualWrite(V5, langkah);
//}

void belok(int x){
  kaki_genap(70, 0);
  kaki_genap(70, 2);
  kaki_genap(70, 4);
  delay(naik_turun);
  kaki_ganjil(90+x, 0);
  kaki_ganjil(90+x, 2);
  kaki_ganjil(90+x, 4);
  kaki_ganjil(90-x, 1);
  kaki_ganjil(90-x, 3);
  kaki_ganjil(90-x, 5);
  delay(naik_turun);
  kaki_genap(110, 0);
  kaki_genap(110, 2);
  kaki_genap(110, 4);
  delay(delay1);
  
  kaki_genap(70, 1);
  kaki_genap(70, 3);
  kaki_genap(70, 5);
  delay(naik_turun);
  kaki_ganjil(90-x, 0);
  kaki_ganjil(90-x, 2);
  kaki_ganjil(90-x, 4);
  kaki_ganjil(90+x, 1);
  kaki_ganjil(90+x, 3);
  kaki_ganjil(90+x, 5);
  delay(naik_turun);
  kaki_genap(110, 1);
  kaki_genap(110, 3);
  kaki_genap(110, 5);
  delay(delay1);
}

void putar_balik(){
  kaki_genap(70, 0);
  kaki_genap(70, 2);
  kaki_genap(70, 4);
  delay(naik_turun);
  kaki_ganjil(68, 0);
  kaki_ganjil(68, 2);
  kaki_ganjil(68, 4);
  kaki_ganjil(113, 1);
  kaki_ganjil(113, 3);
  kaki_ganjil(113, 5);
  delay(naik_turun);
  kaki_genap(110, 0);
  kaki_genap(110, 2);
  kaki_genap(110, 4);
  delay(delay1);
  
  kaki_genap(70, 1);
  kaki_genap(70, 3);
  kaki_genap(70, 5);
  delay(naik_turun);
  kaki_ganjil(113, 0);
  kaki_ganjil(113, 2);
  kaki_ganjil(113, 4);
  kaki_ganjil(68, 1);
  kaki_ganjil(68, 3);
  kaki_ganjil(68, 5);
  delay(naik_turun);
  kaki_genap(110, 1);
  kaki_genap(110, 3);
  kaki_genap(110, 5);
  delay(delay1);
}

void mundur() {
  kaki_genap(70, 0);
  kaki_genap(70, 2);
  kaki_genap(70, 4);
  delay(naik_turun);
  kaki_ganjil(60, 0);
  kaki_ganjil(90, 2);
  kaki_ganjil(120, 4);
  kaki_ganjil(90, 1);
  kaki_ganjil(90, 3);
  kaki_ganjil(60, 5);
  delay(naik_turun);
  kaki_genap(110, 0);
  kaki_genap(110, 2);
  kaki_genap(110, 4);
  delay(delay1);
  
  kaki_genap(70, 1);
  kaki_genap(70, 3);
  kaki_genap(70, 5);
  delay(naik_turun);
  kaki_ganjil(90, 0);
  kaki_ganjil(120, 2);
  kaki_ganjil(90, 4);
  kaki_ganjil(60, 1);
  kaki_ganjil(120, 3);
  kaki_ganjil(90, 5);
  delay(naik_turun);
  kaki_genap(110, 1);
  kaki_genap(110, 3);
  kaki_genap(110, 5);
  delay(delay1);
}

void maju() {
  kaki_genap(70, 0);
  kaki_genap(70, 2);
  kaki_genap(70, 4);
  delay(naik_turun);
  kaki_ganjil(90, 0);
  kaki_ganjil(120, 2);
  kaki_ganjil(90, 4);
  kaki_ganjil(60, 1);
  kaki_ganjil(120, 3);
  kaki_ganjil(90, 5);
  delay(naik_turun);
  kaki_genap(110, 0);
  kaki_genap(110, 2);
  kaki_genap(110, 4);
  delay(delay1);
  
  kaki_genap(70, 1);
  kaki_genap(70, 3);
  kaki_genap(70, 5);
  delay(naik_turun);
  kaki_ganjil(60, 0);
  kaki_ganjil(90, 2);
  kaki_ganjil(120, 4);
  kaki_ganjil(90, 1);
  kaki_ganjil(90, 3);
  kaki_ganjil(60, 5);
  delay(naik_turun);
  kaki_genap(110, 1);
  kaki_genap(110, 3);
  kaki_genap(110, 5);
  delay(delay1);
}

void berhenti(){
  kaki_genap(90, 0);
  kaki_genap(90, 1);
  kaki_genap(90, 2);
  kaki_genap(90, 3);
  kaki_genap(90, 4);
  kaki_genap(90, 5);
  kaki_ganjil(90, 0);
  kaki_ganjil(90, 1);
  kaki_ganjil(90, 2);
  kaki_ganjil(90, 3);
  kaki_ganjil(90, 4);
  kaki_ganjil(90, 5);
}
void kaki_genap(int sudut, int posisi) {
  if (posisi == 1) sudut -= 15;
  else if (posisi == 2) sudut -= 15;
  else if (posisi == 4) sudut -= 5;
  else if (posisi == 5) sudut -= 10;
  servo = map(sudut, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(2*posisi, 0, servo);
}

void kaki_ganjil(int sudut, int posisi) {
  if (posisi == 0) sudut -= 13;
  else if (posisi == 1) sudut -= 12;
  else if (posisi == 2) sudut += 10;
  else if (posisi == 3) sudut += 10;
  else if (posisi == 4) sudut -= 12;
  servo = map(sudut, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(2*posisi + 1, 0, servo);
}

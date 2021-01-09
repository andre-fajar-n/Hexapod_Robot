//-------GABUNG-----//
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

int pinAnalog1 = A0; //inisialisasi pin sensorM1
int pinAnalog2 = A1; //inisialisasi pin sensorM2
int pinAnalog3 = A2; //inisialisasi pin sensorM3
int pinAnalog4 = A3; //inisialisasi pin sensorM4
int dataAnalog1 = 0;
int dataAnalog2 = 0;
int dataAnalog3 = 0;
int dataAnalog4 = 0;
int adc1 = analogRead(A0);
int adc2 = analogRead(A1);
int adc3 = analogRead(A2);
int adc4 = analogRead(A3);
int relay1 = 2;       //inisialisasi pin relayvalve1
int relay2 = 3;       //inisialisasi pin relayvalve2
int relay3 = 4;       //inisialisasi pin relayvalve3
int relay4 = 5;       //inisialisasi pin relayvalve4
const int relay5 = 6;       //inisialiasasi relay pompa
const int trigPin = 8;     //inisialiasasi pin sensor
const int echoPin = 7;     //inisialiasasi pin sensor
unsigned long previousMillis = 0;        //1000 = 1 detik
unsigned long interval1 = 5000;
unsigned long interval2 = 5000;      
unsigned long interval3 = 4000;
unsigned long interval4 = 6000;
unsigned long interval5 = 8000;


void setup() {
    Serial.begin(9600); //memanggil serial monitor
    pinMode(pinAnalog1, INPUT);   //menetapkan pin A0 sebagai input
    pinMode(pinAnalog2, INPUT);   //menetapkan pin A1 sebagai input
    pinMode(pinAnalog3, INPUT);   //menetapkan pin A2 sebagai input
    pinMode(pinAnalog4, INPUT);   //menetapkan pin A3 sebagai input
    pinMode(relay1, OUTPUT);      //menetapkan pin 2 sebagai output
    pinMode(relay2, OUTPUT);      //menetapkan pin 3 sebagai output
    pinMode(relay3, OUTPUT);       //menetapkan pin 4 sebagai output
    pinMode(relay4, OUTPUT);       //menetapkan pin 5 sebagai output
    pinMode (relay5, OUTPUT);       //menetapkan pin 6 sebagai output       
    pinMode (trigPin, OUTPUT);     //menetapkan pin 8 sebagai output
    pinMode (echoPin, INPUT);      //menetapkan pin 7 sebagai input
    lcd.begin(16,2);
    lcd.init();
    lcd.backlight();
}

void loop() {
//---------------SENSOR ULTRASONIK-----------------

//------------SENSOR KELEMBAPAN DAN LCD------------
//-----------------TAMPILAN LCD 1------------------
    if (adc1=analogRead(A0)){
        float volt1 = adc1 * (5.0 / 1023);
        lcd.setCursor(0,0);
        lcd.print("ADC1:");
        lcd.setCursor(5,0);
        lcd.print(adc1);
        Serial.println(adc1);
        Serial.println();
        lcd.setCursor(9,0);
        lcd.print("V1:");
        Serial.println("V1:");
        lcd.setCursor(12,0);
        lcd.print(volt1);
        Serial.println(volt1);
        lcd.setCursor(0,1);
        lcd.print("status1:");
        Serial.println("status1:");
    }

    //-----------------TAMPILAN LCD 2------------------
    if (adc2=analogRead(A1)){
        float volt2 = adc2 * (5.0 / 1023);
        lcd.setCursor(0,0);
        lcd.print("ADC2:");
        lcd.setCursor(5,0);
        lcd.print(adc2);
        Serial.print(adc2);
        lcd.setCursor(9,0);
        lcd.print("V2:");
        Serial.print("V2:");
        lcd.setCursor(12,0);
        lcd.print(volt2);
        Serial.print(volt2);
        lcd.setCursor(0,1);
        lcd.print("status2:");
        Serial.print("status2:");
    }

    //-----------------TAMPILAN LCD 3------------------
    //-----------------TAMPILAN LCD 4------------------

    //-----------KODINGAN SENSOR KELEMBAPAN 1----------
    if(adc1 > 700){
        lcd.setBacklight(100);
        lcd.setCursor(9,1);
        lcd.print("KERING");
        Serial.println("KERING");
        digitalWrite (relay1, LOW);
    }

    else if(adc1  <= 700 && adc1 > 350) {
        lcd.setBacklight(100);
        lcd.setCursor(9,1);
        lcd.print("CUKUP");
        Serial.println("CUKUP");
        digitalWrite (relay1, HIGH);
    }

    else if(adc1 > 0 && adc1 <=350) {
        lcd.setBacklight(100);
        lcd.setCursor(9,1);
        lcd.print("BASAH ");
        Serial.println("BASAH ");
        digitalWrite (relay1, HIGH);
    }
    
    //-----------KODINGAN SENSOR KELEMBAPAN 2----------
    if(adc2 > 700){
        lcd.setBacklight(100);
        lcd.setCursor(9,1);
        lcd.print("KERING");
        Serial.print("KERING");
        digitalWrite (relay2, LOW);
    } else if(adc2  <= 700 && adc2 > 350) {
        lcd.setBacklight(100);
        lcd.setCursor(9,1);
        lcd.print("CUKUP");
        Serial.print("CUKUP");
        digitalWrite (relay2, HIGH);
    } else if(adc2 > 0 && adc2 <=350) {
        lcd.setBacklight(100);
        lcd.setCursor(9,1);
        lcd.print("BASAH ");
        Serial.print("BASAH ");
        digitalWrite (relay2, HIGH);
    }
//-----------KODINGAN SENSOR KELEMBAPAN 3----------
//-----------KODINGAN SENSOR KELEMBAPAN 4----------
}
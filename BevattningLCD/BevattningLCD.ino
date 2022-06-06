#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#include <EEPROM.h>
#include <dht.h>

dht DHT;

#define DHT22_PIN 31
float t,h;

#define SENSOR1_PIN A1
#define SENSOR2_PIN A2
#define SENSOR3_PIN A3
#define SENSOR4_PIN A4
//#define SENSOR1_AIR 540
//#define SENSOR1_WATER 210

#define PUMP1_PIN 11
#define PUMP2_PIN 12
#define PUMP3_PIN 13
#define PUMP4_PIN 3
//#define PUMP1_TRIGGER 30


#define WIFI_RESET_PIN 22

//int sensorValue = 0;        // value read from the pot
//int sensorValue2 = 0;        // value read from the pot
//int outputValue = 0;        // value output to the PWM (analog out)

//bool pump_state[2];
//bool pump2_state;
//bool pump3_state;
//bool pump4_state;
//int sensor1_value;
//int sensor2_value;
//int sensor3_value;
//int sensor4_value;
//int SENSOR1_AIR = 540;
//int SENSOR_AIR[2] = 540;
//int SENSOR3_AIR = 540;
//int SENSOR4_AIR = 540;
//int SENSOR1_WATER = 210;
//int SENSOR_WATER2] = 210;
//int SENSOR3_WATER = 210;
//int SENSOR4_WATER = 210;
//byte PUMP1_TRIGGER = 30;
//byte PUMP_TRIGGER[2] = 30;
//byte PUMP3_TRIGGER = 30;
//byte PUMP4_TRIGGER = 30;

bool pump_state[5];
int sensor_value[5];
int fukt[5];
int SENSOR_AIR[5] = {210,540,210,30,30};
int SENSOR_WATER[5] = {210,210,210,30,30};
byte PUMP_TRIGGER[5] = {30,30,30,30,30};
bool failstate[5];
long timerpump[5];
long timerfilter[5];

int PUMP_PIN[5] = {0,PUMP1_PIN,PUMP2_PIN,PUMP3_PIN,PUMP4_PIN};
int SENSOR_PIN[5] = {0,SENSOR1_PIN,SENSOR2_PIN,SENSOR3_PIN,SENSOR4_PIN};

void setupSavedata() {
  byte data = EEPROM.read(1);
  if (data == 10) {
    Serial.println("Found savedata");

    SENSOR_AIR[1] = EEPROM.read(2)*4;
    SENSOR_WATER[1] = EEPROM.read(3)*4;
    PUMP_TRIGGER[1] = EEPROM.read(4);

    Serial.print("SENSOR1_AIR");
    Serial.println(SENSOR_AIR[1]);
    Serial.print("SENSOR1_WATER");
    Serial.println(SENSOR_WATER[1]);
    Serial.print("PUMP1_TRIGGER");
    Serial.println(PUMP_TRIGGER[1]);

    SENSOR_AIR[2] = EEPROM.read(5)*4;
    SENSOR_WATER[2] = EEPROM.read(6)*4;
    PUMP_TRIGGER[2] = EEPROM.read(7);

    Serial.print("SENSOR_AIR[2]");
    Serial.println(SENSOR_AIR[2]);
    Serial.print("SENSOR_WATER2]");
    Serial.println(SENSOR_WATER[2]);
    Serial.print("PUMP_TRIGGER[2]");
    Serial.println(PUMP_TRIGGER[2]);
    
  } else {
    Serial.print("Invalid savedata");
    Serial.println(data);
    
    EEPROM.write(1, 10);
    EEPROM.write(2, SENSOR_AIR[1]/4);
    EEPROM.write(3, SENSOR_WATER[1]/4);
    EEPROM.write(4, PUMP_TRIGGER[1]);
    EEPROM.write(5, SENSOR_AIR[2]/4);
    EEPROM.write(6, SENSOR_WATER[2]/4);
    EEPROM.write(7, PUMP_TRIGGER[2]);
  }
  //EEPROM.write(address, value);
}

void writeSaveData() {
  Serial.println("Write savedata");
  
  
  EEPROM.write(1, 10);
  EEPROM.write(2, SENSOR_AIR[1]/4);
  EEPROM.write(3, SENSOR_WATER[1]/4);
  EEPROM.write(4, PUMP_TRIGGER[1]);
  EEPROM.write(5, SENSOR_AIR[2]/4);
  EEPROM.write(6, SENSOR_WATER[2]/4);
  EEPROM.write(7, PUMP_TRIGGER[2]);
}

#include "buttons.h"
#include "lcd.h"

void setup() {
  pinMode(PUMP1_PIN, OUTPUT);
  pinMode(PUMP2_PIN, OUTPUT);
  digitalWrite(PUMP1_PIN,LOW);
  digitalWrite(PUMP2_PIN,LOW);

  pinMode(WIFI_RESET_PIN, OUTPUT);
  digitalWrite(WIFI_RESET_PIN,HIGH);
  
  
  initLCD();
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(1000);
  setupSavedata();
  
}

void loop() {
  btn_loop();
  readDHT();
  drawMenu();

  nypump(1);
  delay(100); 
  nypump(2);

  resetWifi();
//  sensorValue = analogRead(SENSOR1_PIN);
//  outputValue = map(sensorValue, SENSOR1_AIR, SENSOR1_WATER, 0, 100);
//  sensor1_value = constrain(outputValue, 0, 100);
//
//  pump(PUMP1_PIN, PUMP1_TRIGGER, &pump_state[2]);
//  failsafe(PUMP1_PIN, sensorValue, SENSOR1_AIR, SENSOR1_WATER, &pump_state[2], 1);
//  delay(100); 
//   
//  sensorValue2 = analogRead(SENSOR2_PIN);
//  outputValue = map(sensorValue2, SENSOR_AIR[2], SENSOR_WATER2], 0, 100);
//  sensor2_value = constrain(outputValue, 0, 100);  
//  
//  pump(PUMP2_PIN, PUMP_TRIGGER[2], &pump2_state);
//  failsafe(PUMP2_PIN, sensorValue2, SENSOR_AIR[2], SENSOR_WATER2], &pump2_state, 2);

 

//  if (outputValue<PUMP1_TRIGGER) {
//    digitalWrite(PUMP1_PIN,HIGH);
//    pump_state[2] = true;
//  }else {
//    digitalWrite(PUMP1_PIN,LOW);
//    pump_state[2] = false;
//  }

    // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  sendData();

}

void resetWifi() {
  static long resettimer = 0;
  if (millis()>resettimer ) {
    resettimer = millis()+(1000*60*14);
    digitalWrite(WIFI_RESET_PIN,LOW);
    delay(200);
    digitalWrite(WIFI_RESET_PIN,HIGH);
  }else {
    return;
  }
  
  
}

//void pump(int pin, int trigger, bool* state) {
//  if (millis()>timerpump ) {
//    timerpump = millis()+1000;
//  }else {
//    return;
//  }
//  if (outputValue<trigger) {
//    digitalWrite(pin,HIGH);
//    *state = true;
//  }else {
//    digitalWrite(pin,LOW);
//    *state = false;
//  }
//}

void failsafe(int pin, int value, int maxval, int minval, bool* state, int nr) {

  if (value>maxval || value<minval) {
    digitalWrite(pin,LOW);
    *state = false;
    failstate[nr] = true;
  }else {
    
    failstate[nr] = false;
  }
}

void nypump(int nr) {
  
  if (millis()>timerpump[nr] ) {
    timerpump[nr] = millis()+1000;
  }else {
    return;
  }

  sensor_value[nr] = analogRead(SENSOR_PIN[nr]);
  int oV = map(sensor_value[nr], SENSOR_AIR[nr], SENSOR_WATER[nr], 0, 100);
  fukt[nr] = constrain(oV, 0, 100);

  // FAILSAFE
  if (sensor_value[nr]>SENSOR_AIR[nr] || sensor_value[nr]<SENSOR_WATER[nr]) {
    digitalWrite(PUMP_PIN[nr],LOW);
    pump_state[nr] = false;
    failstate[nr] = true;
    timerfilter[nr] = 0;
    return;
  }else {
    failstate[nr] = false;
  }

  if (fukt[nr]<PUMP_TRIGGER[nr]) {
    if (filterFunction(nr) == 1) {
      digitalWrite(PUMP_PIN[nr],HIGH);
      pump_state[nr] = true;
    }
    
  }else {
    timerfilter[nr] = 0;
    digitalWrite(PUMP_PIN[nr],LOW);
    pump_state[nr] = false;
  }
}

int filterFunction(int nr) {
  timerfilter[nr] = timerfilter[nr]+1; // anroppas en gång per sekund
  if (timerfilter[nr]>(60*5) ) { // fuktmätaren måste vara inom godtyckligt triggervärde i 5 minuter innan pumpen aktiveras
    return 1;
  }
  return 0;
}

void sendData() {
  static long timersend = 0;
  if (millis()>timersend ) {
    timersend = millis()+1000;
  }else {
    return;
  }
  Serial.print("{\"s1\":");
  Serial.print(fukt[1]);
  Serial.println("}");

  Serial.print("{\"p1\":");
  Serial.print(pump_state[1]);
  Serial.println("}");

  Serial.print("{\"t1\":");
  Serial.print(t);
  Serial.println("}");

  Serial.print("{\"t2\":");
  Serial.print(h);
  Serial.println("}");
 
}

void readDHT() {
  static long timersend1 = 0;
  if (millis()>timersend1 ) {
    timersend1 = millis()+1100;
  }else {
    return;
  }
  int readData = DHT.read22(DHT22_PIN);
  t = DHT.temperature;
  h = DHT.humidity;
}

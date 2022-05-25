#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#include <EEPROM.h>
#include <dht.h>

dht DHT;

#define DHT22_PIN 31
float t,h;

#define SENSOR1_PIN A1
#define SENSOR2_PIN A2
//#define SENSOR1_AIR 540
//#define SENSOR1_WATER 210

#define PUMP1_PIN 11
#define PUMP2_PIN 12
//#define PUMP1_TRIGGER 30

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

bool pump1_state;
int sensor1_value;
int SENSOR1_AIR = 540;
int SENSOR1_WATER = 210;
byte PUMP1_TRIGGER = 30;

#include "buttons.h"
#include "lcd.h"

void setup() {
  pinMode(PUMP1_PIN, OUTPUT);
  pinMode(PUMP2_PIN, OUTPUT);
  digitalWrite(PUMP1_PIN,LOW);
  digitalWrite(PUMP2_PIN,LOW);
  
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
  // read the analog in value:
  sensorValue = analogRead(SENSOR1_PIN);
  // map it to the range of the analog out:
  outputValue = map(sensorValue, SENSOR1_AIR, SENSOR1_WATER, 0, 100);
  sensor1_value = constrain(outputValue, 0, 100);
  // change the analog out value:

//  if (outputValue<PUMP1_TRIGGER) {
//    digitalWrite(PUMP1_PIN,HIGH);
//    pump1_state = true;
//  }else {
//    digitalWrite(PUMP1_PIN,LOW);
//    pump1_state = false;
//  }
  pump(PUMP1_PIN, PUMP1_TRIGGER, &pump1_state);
    // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  sendData();
  delay(2);
}

long timerpump;
void pump(int pin, int trigger, bool* state) {
  if (millis()>timerpump ) {
    timerpump = millis()+1000;
  }else {
    return;
  }
  if (outputValue<trigger) {
    digitalWrite(pin,HIGH);
    *state = true;
  }else {
    digitalWrite(pin,LOW);
    *state = false;
  }
}


void setupSavedata() {
  byte data = EEPROM.read(1);
  if (data == 10) {
    Serial.println("Found savedata");

    SENSOR1_AIR = EEPROM.read(2)*4;
    SENSOR1_WATER = EEPROM.read(3)*4;
    PUMP1_TRIGGER = EEPROM.read(4);

    Serial.print("SENSOR1_AIR");
    Serial.println(SENSOR1_AIR);
    Serial.print("SENSOR1_WATER");
    Serial.println(SENSOR1_WATER);
    Serial.print("PUMP1_TRIGGER");
    Serial.println(PUMP1_TRIGGER);
    
  } else {
    Serial.print("Invalid savedata");
    Serial.println(data);
    
    EEPROM.write(1, 10);
    EEPROM.write(2, SENSOR1_AIR/4);
    EEPROM.write(3, SENSOR1_WATER/4);
    EEPROM.write(4, PUMP1_TRIGGER);
  }
  //EEPROM.write(address, value);
}

void sendData() {
  static long timersend = 0;
  if (millis()>timersend ) {
    timersend = millis()+1000;
  }else {
    return;
  }
  Serial.print("{\"s1\":");
  Serial.print(sensor1_value);
  Serial.println("}");

  Serial.print("{\"p1\":");
  Serial.print(pump1_state);
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

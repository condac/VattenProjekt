//#include<avr/wdt.h>

//#include <Ethernet.h>
//#include <WiFiEsp.h>
//#include <WiFiEspClient.h>
#include <WiFiNINA.h>
#include <ArduinoHA.h>

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 7     // Digital pin connected to the DHT sensor 
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
//#define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);


#define PUMP_PIN1 8
#define PUMP_PIN2 9
#define PUMP_PIN3 10
#define PUMP_PIN4 11


#define MARKFUKT_PIN1 A0
#define MARKFUKT_PIN2 A1
#define MARKFUKT_PIN3 A2
#define MARKFUKT_PIN4 A3

#define BROKER_ADDR IPAddress(192,168,0,16)
#include "secret.h"
//#define BROKER_USERNAME     "moved" // replace with your credentials
//#define BROKER_PASSWORD     "moved" // in secret.h

byte mac[] = {0x00, 0x10, 0xFA, 0x6E, 0x38, 0x4A};
unsigned long lastSentAt = millis();
double lastValue = 0;

uint16_t triggerval1 = 50;
uint16_t triggerval2 = 50;
uint16_t triggerval3 = 50;
uint16_t triggerval4 = 50;
bool pumpstate1 = false;
bool pumpstate2 = false;
bool pumpstate3 = false;
bool pumpstate4 = false;

int sensor1;
int sensor2;
//EthernetClient client;

//char ssid[] = "moved"; // replace with your credentials
//char pwd[] = "moved";  // in secret.h

//WiFiEspClient client;
WiFiClient client;
HADevice device(mac, sizeof(mac));
HAMqtt mqtt(client, device);

HASensor temp("temp"); // "temp" is unique ID of the sensor. You should define your own ID.
HASensor fukt("fukt"); // "temp" is unique ID of the sensor. You should define your own ID.
HASwitch pump1("pump1", false); // "led" is unique ID of the switch. You should define your own ID.
HASwitch pump2("pump2", false); // "led" is unique ID of the switch. You should define your own ID.
HASwitch pump3("pump3", false); // "led" is unique ID of the switch. You should define your own ID.
HASwitch pump4("pump4", false); // "led" is unique ID of the switch. You should define your own ID.
HASensor markfukt1("markfukt1"); // "temp" is unique ID of the sensor. You should define your own ID.
HASensor markfukt2("markfukt2"); // "temp" is unique ID of the sensor. You should define your own ID.
HASensor markfukt3("markfukt3"); // "temp" is unique ID of the sensor. You should define your own ID.
HASensor markfukt4("markfukt4"); // "temp" is unique ID of the sensor. You should define your own ID.
HAFan trigger1("trigger1", HAFan::SpeedsFeature);




void(* resetFunc) (void) = 0;

void onBeforeSwitchStateChanged(bool state, HASwitch* s) {
    // this callback will be called before publishing new state to HA
    // in some cases there may be delay before onStateChanged is called due to network latency
}

void onSwitchStateChanged1(bool state, HASwitch* s) {
    Serial.print("pump1 changed");
    Serial.println(state);
    //digitalWrite(PUMP_PIN1, (state ? HIGH : LOW));
    if (s == &pump1) {
      pumpstate1 = state;
      digitalWrite(PUMP_PIN1, (state ? HIGH : LOW));
    }
    if (s == &pump2) {
      pumpstate2 = state;
      digitalWrite(PUMP_PIN2, (state ? HIGH : LOW));
    }
    if (s == &pump3) {
      pumpstate3 = state;
      digitalWrite(PUMP_PIN3, (state ? HIGH : LOW));
    }
    if (s == &pump4) {
      pumpstate4 = state;
      digitalWrite(PUMP_PIN4, (state ? HIGH : LOW));
    }
   
}
void onStateChanged(bool state) {
  Serial.print("State: ");
  Serial.println(state);
}

void onSpeedChanged(uint16_t speed) {
  triggerval1 = speed;
  Serial.print("Speed: ");
  Serial.println(speed);
}

bool wifiConnected = false;

int connectWifi() {
  if (wifiConnected) {
    return wifiConnected;
  }
  else {
    //wdt_disable();
    WiFi.begin(ssid, pwd);
    if(strcmp(WiFi.SSID(), ssid) == 0) {
      Serial.println("Wifi anslutet");
      wifiConnected = true;
      mqtt.begin(BROKER_ADDR, BROKER_USERNAME, BROKER_PASSWORD);
      //wdt_reset();
      WiFi.status();
    }
    else {
      Serial.println("Wifi ej anslutet");
      wifiConnected = false;
    }
    //wdt_reset();
    //wdt_enable(WDTO_8S);
  }

  //wdt_reset();
  return wifiConnected;
}

void setup() {
  //wdt_reset();
  //wdt_disable();
  pinMode(PUMP_PIN1, OUTPUT);
  pinMode(PUMP_PIN2, OUTPUT);
  pinMode(PUMP_PIN3, OUTPUT);
  pinMode(PUMP_PIN4, OUTPUT);
  digitalWrite(PUMP_PIN1, LOW);
  digitalWrite(PUMP_PIN2, LOW);
  digitalWrite(PUMP_PIN3, LOW);
  digitalWrite(PUMP_PIN4, LOW);
    // you don't need to verify return status
    //Ethernet.begin(mac);
    Serial1.begin(9600);
    Serial.begin(115200);
    Serial.println("boot");
    delay(1000);
    while (Serial1.available()) {
      char c = Serial1.read();
      Serial.write(c);
    }
    //wdt_enable(WDTO_8S);
    //wdt_reset();
    //WiFi.init(&Serial1);
    //wdt_reset();
    //WiFi.Multi();
    //wdt_reset();
    connectWifi();

    // set device's details (optional)
    device.setName("arduino/boden/");
    device.setSoftwareVersion("1.0.0");

    // configure sensor (optional)
    temp.setUnitOfMeasurement("°C");
    temp.setDeviceClass("temperature");
    temp.setIcon("mdi:home");
    temp.setName("Boden Tempratur");
    // configure sensor (optional)
    fukt.setUnitOfMeasurement("%");
    fukt.setDeviceClass("humidity");
    fukt.setIcon("mdi:home");
    fukt.setName("Boden Luftfuktighet");


    // set icon (optional)
    pump1.setIcon("mdi:lightbulb");
    pump1.setName("Pump1");
    pump1.onBeforeStateChanged(onBeforeSwitchStateChanged); // optional
    pump1.onStateChanged(onSwitchStateChanged1);
    // set icon (optional)
    pump2.setIcon("mdi:lightbulb");
    pump2.setName("Pump2");
    pump2.onBeforeStateChanged(onBeforeSwitchStateChanged); // optional
    pump2.onStateChanged(onSwitchStateChanged1);
    // set icon (optional)
    pump3.setIcon("mdi:lightbulb");
    pump3.setName("Pump3");
    pump3.onBeforeStateChanged(onBeforeSwitchStateChanged); // optional
    pump3.onStateChanged(onSwitchStateChanged1);
    // set icon (optional)
    pump4.setIcon("mdi:lightbulb");
    pump4.setName("Pump4");
    pump4.onBeforeStateChanged(onBeforeSwitchStateChanged); // optional
    pump4.onStateChanged(onSwitchStateChanged1);

    // configure sensor (optional)
    markfukt1.setUnitOfMeasurement("%");
    markfukt1.setDeviceClass("humidity");
    markfukt1.setIcon("mdi:home");
    markfukt1.setName("Boden Markfukt1");

    // configure sensor (optional)
    markfukt2.setUnitOfMeasurement("%");
    markfukt2.setDeviceClass("humidity");
    markfukt2.setIcon("mdi:home");
    markfukt2.setName("Boden Markfukt2");

    // configure sensor (optional)
    markfukt3.setUnitOfMeasurement("%");
    markfukt3.setDeviceClass("humidity");
    markfukt3.setIcon("mdi:home");
    markfukt3.setName("Boden Markfukt3");

    // configure sensor (optional)
    markfukt4.setUnitOfMeasurement("%");
    markfukt4.setDeviceClass("humidity");
    markfukt4.setIcon("mdi:home");
    markfukt4.setName("Boden Markfukt4");

    // configure fan (optional)
    trigger1.setName("Trigger1");
    trigger1.setRetain(true);
    trigger1.setSpeedRangeMin(1);
    trigger1.setSpeedRangeMax(100);
    // handle fan states
    trigger1.onStateChanged(onStateChanged);
    trigger1.onSpeedChanged(onSpeedChanged);

    //wdt_reset();
    
    //wdt_reset();
}

float t;
float h;

void loop() {
  
  //wdt_reset();
  if (connectWifi()) {
    int constatus = mqtt.isConnected();
    mqtt.loop();
    //wdt_reset();
    if (constatus == 1) {
      //wdt_reset();
      //mqtt.loop();
      //wdt_reset();
    }else {
      Serial.println("Not connected");
     // wdt_reset();
      int ws = WiFi.status();
      //wdt_reset();
      Serial.print("status");
      Serial.println(ws);
      if (ws == 4) {
        Serial.println("Not connected 4 trigger reset");
        delay(10);
        resetFunc();
        assertEquals("Check SSID", WiFi.SSID(), ssid);
        if(strcmp(WiFi.SSID(), ssid) == 0) {
          Serial.println("Wifi anslutet");
        }
        else {
          Serial.println("Wifi ej anslutet");
          mqtt.disconnect();
          //WiFi.init(&Serial1);
          //WiFi.Multi();
          WiFi.begin(ssid, pwd);
          mqtt.begin(BROKER_ADDR, BROKER_USERNAME, BROKER_PASSWORD);
        }
      }
      if (ws == 5) {
        assertEquals("Check SSID", WiFi.SSID(), ssid);
        if(strcmp(WiFi.SSID(), ssid) == 0) {
          Serial.println("Wifi anslutet");
        }
        else {
          Serial.println("Not connected 5 trigger reset");
          delay(10);
          resetFunc();
        }
      }
      if (ws == 2) {
        Serial.println("Not connected 2 trigger reset");
        delay(10);
        //resetFunc();
        Serial.println("Försöker starta om mqtt anslutningen");

      }

      ws = WiFi.status();
      Serial.print("status");
      Serial.println(ws);
    }
  }
  
  //wdt_reset();
  if ((millis() - lastSentAt) >= 5000) {
    //pumpLoop();
    //wdt_reset();
//    readData = DHT.read22(DHT_DATA);
//    t = DHT.temperature;
//    h = DHT.humidity;
    lastSentAt = millis();
    lastValue = lastValue + 0.5;
    temp.setValue(t);
    fukt.setValue(h);

    float fukt = sensor1;
    markfukt1.setValue(fukt);
    fukt = sensor2;
    markfukt2.setValue(fukt);
    fukt = analogRead(MARKFUKT_PIN3);
    markfukt3.setValue(fukt/10.24);
    fukt = analogRead(MARKFUKT_PIN4);
    markfukt4.setValue(fukt/10.24);

    pump1.setState(pumpstate1, 0);

    Serial.print("Temp:");
    Serial.print(t);
    Serial.print(" fukt:");
    Serial.println(h);
    // Supported data types:
    // uint32_t (uint16_t, uint8_t)
    // int32_t (int16_t, int8_t)
    // double
    // float
    // const char*
  }
  readData();
}


void pumpLoop() {

  float fukt = analogRead(MARKFUKT_PIN1);
  if (fukt/10.24 < triggerval1) {
    Serial.println("pumploop true");
    if (!pumpstate1) {
      Serial.println("pumploop set 1 true");
      pumpstate1 = true;
      pump1.setState(true, 0);
    }
      
  }else {
    
    Serial.println("pumploop false");
    if (pumpstate1) {
      Serial.println("pumploop set 1 false");
      pumpstate1 = false;
      pump1.setState(false, 0);
    }
  }
 
}


void readData() {
  
  while (Serial1.available()) {
    char inChar = (char)Serial1.read();
    
    if (inChar == '{') {
      
      //Serial.println("start");
      delay(10);
      
      inChar = (char)Serial1.read();
      if (inChar == '"') {
       // Serial.println("citat");
        
        inChar = (char)Serial1.read();
        if (inChar == 's') {
          
          //Serial.println("ess");
          int nr = Serial1.parseInt();
          int value = Serial1.parseInt();
          if (nr == 1) {
            sensor1 = value;
            Serial.print("sensor1 recieved ");
            Serial.println(sensor1);
          } 
          if (nr == 2) {
            sensor2 = value;
            Serial.print("sensor2 recieved ");
            Serial.println(sensor2);
          } 
        }
        if (inChar == 'p') {
          
          
          int nr = Serial1.parseInt();
          int value = Serial1.parseInt();
          if (nr == 1) {
            pumpstate1 = value;
            Serial.print("pump1 recieved ");
            Serial.println(pumpstate1);
          } 
          if (nr == 2) {
            pumpstate2 = value;
            Serial.print("pumpstate2 recieved ");
            Serial.println(pumpstate2);
          } 
        }
        if (inChar == 't') {
          int nr = Serial1.parseInt();
          float value = Serial1.parseFloat();
          if (nr == 1) {
            t = value;
            Serial.print("temp recieved ");
            Serial.println(t);
          } 
          if (nr == 2) {
            h = value;
            Serial.print("humid recieved ");
            Serial.println(h);
          } 
        }
      }
    }else {
      //Serial.print(inChar);
    }
  }
}

void assertNotEquals(const char* test, int actual, int expected)
{
  if(actual!=expected)
    pass(test);
  else
    fail(test, actual, expected);
}

void assertEquals(const char* test, int actual, int expected)
{
  if(actual==expected)
    pass(test);
  else
    fail(test, actual, expected);
}

void assertEquals(const char* test, char* actual, char* expected)
{
  if(strcmp(actual, expected) == 0)
    pass(test);
  else
    fail(test, actual, expected);
}
void assertEquals(const char* test, const char* actual, char* expected)
{
  if(strcmp(actual, expected) == 0)
    pass(test);
  else
    fail(test, actual, expected);
}

void pass(const char* test)
{
  Serial.print(F("********************************************** "));
  Serial.print(test);
  Serial.println(" > PASSED");
  Serial.println();
}

void fail(const char* test, char* actual, char* expected)
{
  Serial.print(F("********************************************** "));
  Serial.print(test);
  Serial.print(" > FAILED");
  Serial.print(" (actual=\"");
  Serial.print(actual);
  Serial.print("\", expected=\"");
  Serial.print(expected);
  Serial.println("\")");
  Serial.println();
  delay(100);
}
void fail(const char* test,const char* actual, char* expected)
{
  Serial.print(F("********************************************** "));
  Serial.print(test);
  Serial.print(" > FAILED");
  Serial.print(" (actual=\"");
  Serial.print(actual);
  Serial.print("\", expected=\"");
  Serial.print(expected);
  Serial.println("\")");
  Serial.println();
  delay(100);
}
void fail(const char* test, int actual, int expected)
{
  Serial.print(F("********************************************** "));
  Serial.print(test);
  Serial.print(" > FAILED");
  Serial.print(" (actual=");
  Serial.print(actual);
  Serial.print(", expected=");
  Serial.print(expected);
  Serial.println(")");
  Serial.println();
  delay(100);
}

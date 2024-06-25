#include "DHT.h"

const byte DHT_PIN        = 4;     
const byte SOIL_PIN       = 34;
const byte WATERLEVEL_PIN = 35;
const byte LDR_PIN        = 26; 
const byte WATERDROP_PIN  = 25; 
const byte PIR_PIN        = 27;  
const byte RELAY_1_PIN    = 13;
const byte RELAY_2_PIN    = 12;
const byte RELAY_3_PIN    = 5;
const byte RELAY_4_PIN    = 23; 
const byte BUZZER_PIN     = 14;   

#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE); 
float temperatureC, humidity;
int16_t valuesoil, valuesoil_new, valuewaterlevel, valuewaterlevel_new;
int16_t dataLDR, data_waterdrop;
bool data_pir = 0, pirState = LOW;

void component_setup(){
  dht.begin();
  pinMode(PIR_PIN, INPUT);
  pinMode(LDR_PIN, INPUT); 
  pinMode(WATERDROP_PIN, INPUT); 
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_1_PIN, OUTPUT);
  pinMode(RELAY_2_PIN, OUTPUT);
  pinMode(RELAY_3_PIN, OUTPUT);
  pinMode(RELAY_4_PIN, OUTPUT);
}

void sensor_read(){
  humidity = dht.readHumidity();
  temperatureC = dht.readTemperature();
  valuesoil = analogRead(SOIL_PIN); 
  valuesoil_new = map(valuesoil, 0, 4095, 100, 0);
  valuewaterlevel = analogRead(WATERLEVEL_PIN); 
  valuewaterlevel_new = map(valuewaterlevel, 0, 4500, 0, 100);
  dataLDR = digitalRead(LDR_PIN);
  data_waterdrop = digitalRead(WATERDROP_PIN);
  data_pir = digitalRead(PIR_PIN);  
  if (data_pir == HIGH) {            
    if (pirState == LOW) {
      Serial.println("Motion detected!");
      digitalWrite(BUZZER_PIN, HIGH);
      delay(3000);
      pirState = HIGH;
    }
  } else {
    if (pirState == HIGH){
      Serial.println("Motion ended!");
      digitalWrite(BUZZER_PIN, LOW);
      delay(3000);
      pirState = LOW;
    }
  }
}
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   
#include <ArduinoJson.h>
#include "Component.h"

#define BOTtoken "6191949183:AAEMpb15epA24iZfddCG9jUUGJjAMAjHHGo"  
// #define CHAT_ID "1047508852"
#define CHAT_ID "1448068007"
const char* ssid = "Greenroof";
const char* password = "goldmedal";

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
int16_t botRequestDelay = 1000;
unsigned long lastTimeBotRan;

void setup() {
  Serial.begin(115200);
  component_setup();

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");    
    client.setTrustAnchors(&cert); 
  #endif

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); 
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());
}

void loop() {
  sensor_read();

  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + " to The Green Roof System. ";
      welcome += "Check the condition of your Rooftop Garden!\n";
      welcome += "The system will immediately show the condition of your Rooftop Garden. Use the following command to get current condition!\n";
      welcome += "/RoofGarden \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/RoofGarden") {
      String msgReading = "Main Menu\n";
      msgReading += "/ActivateSensors\n";
      msgReading += "/SwitchButtonMenu\n";
      bot.sendMessage(chat_id, msgReading, "");
    }

    if (text == "/ActivateSensors"){
      String msgSensors1;
      String msgSensors2;
      String msgSensors3;
      String msgSensors4;
      String msgSensors5;
      String msgSensors6;
      msgSensors1 = "Temperature : ";
      msgSensors1 += msgSensors1.concat(temperatureC);
      msgSensors1 += "°C";
      bot.sendMessage(chat_id, msgSensors1, "");
      msgSensors2 = "\nHumidity : ";
      msgSensors2 += msgSensors2.concat(humidity);
      msgSensors2 += "%";
      bot.sendMessage(chat_id, msgSensors2, ""); 
      if (valuesoil_new >= 36){
        msgSensors3 = "\nThe soil : WET";
      }else if(valuesoil_new >= 10 && valuesoil_new <= 35){
        msgSensors3 = "\nThe soil : HUMID";
      }else{
        msgSensors3 = "\nThe soil : DRY";
      }    
      bot.sendMessage(chat_id, msgSensors3, "");  
      msgSensors4 = "\nValue of Water : ";
      msgSensors4 += msgSensors4.concat(valuewaterlevel_new);
      msgSensors4 += "%"; 
      bot.sendMessage(chat_id, msgSensors4, "");
      if (data_waterdrop == 0){
        msgSensors5 = "\nRainy : Yes";
      }else{
        msgSensors5 = "\nRainy : No";
      }
      bot.sendMessage(chat_id, msgSensors5, "");
      if (dataLDR == 0){
        msgSensors6 = "\nTime : Day";
      }else{
        msgSensors6 = "\nTime : Night";
      }
      bot.sendMessage(chat_id, msgSensors6, "");
    }

    if(text == "/SwitchButtonMenu"){
      String msgReading2= "Switch Button Menu\n";
      msgReading2 += "/WaterPump_on \n";
      msgReading2 += "/WaterPump_off \n";
      msgReading2 += "/LiquidFertilizer_pump_on \n";
      msgReading2 += "/LiquidFertilizer_pump_off \n";
      msgReading2 += "/Growlight_Lamp_on \n";
      msgReading2 += "/Growlight_Lamp_off \n";
      bot.sendMessage(chat_id, msgReading2, "");
    }
    if(text == "/WaterPump_on"){
      String msgWP1 = "Water Pump is On\n";
      msgWP1 += "To turn off the water pump use the following command\n";
      msgWP1 += "/WaterPump_off";
      digitalWrite(RELAY_1_PIN, HIGH);
      bot.sendMessage(chat_id, msgWP1, "");
      
    }
    if(text == "/WaterPump_off"){
      String msgWP2 = "Water Pump is Off\n";
      msgWP2 += "To turn on the water pump use the following command\n";
      msgWP2 += "/WaterPump_on";
      digitalWrite(RELAY_1_PIN, LOW);
      bot.sendMessage(chat_id, msgWP2, "");
    }
    if(text == "/LiquidFertilizer_pump_on"){
      String msgWP3 = "Liquid Fertilizer Pump is On\n";
      msgWP3 += "To turn off the liquid fertilizer pump use the following command\n";
      msgWP3 += "/LiquidFertilizer_pump_off";
      digitalWrite(RELAY_2_PIN, HIGH);
      bot.sendMessage(chat_id, msgWP3, "");
    }
    if(text == "/LiquidFertilizer_pump_off"){
      String msgWP4 = "Liquid Fertilizer Pump is Off\n";
      msgWP4 += "To turn on the liquid fertilizer pump use the following command\n";
      msgWP4 += "/LiquidFertilizer_pump_on";
      digitalWrite(RELAY_2_PIN, LOW);
      bot.sendMessage(chat_id, msgWP4, "");
    }
    if(text == "/Growlight_Lamp_on"){
      String msgWP5 = "Lamp is On\n";
      msgWP5 += "To turn off the lamp use the following command\n";
      msgWP5 += "/Growlight_Lamp_off";
      digitalWrite(RELAY_3_PIN, HIGH);
      bot.sendMessage(chat_id, msgWP5, "");
    }
    if(text == "/Growlight_Lamp_off"){
      String msgWP6 = "Lamp is Off\n";
      msgWP6 += "To turn on the lamp use the following command\n";
      msgWP6 += "/Growlight_Lamp_on";
      digitalWrite(RELAY_3_PIN, LOW);
      bot.sendMessage(chat_id, msgWP6, "");
    }
  }
}


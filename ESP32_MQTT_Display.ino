#include <WiFi.h>
#include <MQTTClient.h>
#include <Wire.h> 
#include <ESP32_LiquidCrystal_I2C.h>

#define SDA 15
#define SCL 14
#define COLUMNS 16
#define ROWS    2
#define CLIENT_NAME "LCD_Screen"

String Unidad = "m3";
String Title = "  Medidor";
char* mqtt_user = "mqtt";
char* mqtt_password = "mqtt";
char* mqtt_topic = "main/value";
char* mqtt_server_ip = "";
int mqtt_server_port = 1883;
char* wifi_ssid = "";
char* wifi_password = "";

LiquidCrystal_I2C lcd(0x27,COLUMNS,ROWS); 

WiFiClient net;
MQTTClient client;

void connect();

void PrintOnLCD(String Text)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(Title);
  lcd.setCursor(0,1);
  lcd.print(Text);  
}

void setup() 
{
  Serial.begin(9600);
  
  WiFi.begin(wifi_ssid, wifi_password);
  client.begin(mqtt_server_ip,mqtt_server_port, net);
  client.onMessage(messageReceived);

  Wire.begin(SDA, SCL);
  lcd.begin();                 
  lcd.backlight();
  
  connectToWiFiAndBroker();
}

void connectToWiFiAndBroker() 
{
  PrintOnLCD("Starting WiFi...");
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(1000);
  }
  PrintOnLCD("Conected!");
  
  delay(2000);
  PrintOnLCD("Starting Broker...");
  while (!client.connect(CLIENT_NAME, mqtt_user, mqtt_password)) 
  {
    delay(1000);
  }
  PrintOnLCD("Conected!");
  
  delay(2000);
  PrintOnLCD("--------");
  client.subscribe(mqtt_topic);
}

void loop() 
{
  client.loop();

  if (!client.connected()) 
  {
    connectToWiFiAndBroker();
  }
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) 
{
  PrintOnLCD(payload + ' ' + Unidad);
}

void messageReceived(String &topic, String &payload) 
{
  PrintOnLCD(payload + ' ' + Unidad);
}

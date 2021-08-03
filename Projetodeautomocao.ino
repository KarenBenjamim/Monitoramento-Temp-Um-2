//----Biblioteca----
#include <Arduino.h>
#include <WiFi.h>
#include <LiquidCrystal.h>
#include <DHT.h>
#include <PubSubClient.h>
#include <RTClib.h>
#include <Wire.h>

//----DHT11----
#include <DHT.h>
DHT dht(27, DHT11);
float Temp = 0;
float Umi = 0;

//----RTC----
RTC_DS1307 rtc; // criando a instância RTC
char diasDaSemana[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sabado"};

int segundo = 0;
int minuto = 38;
int hora = 11;
String dia = "Terça";
int data = 8;
int mes = 12;
int ano = 2020;

//----LCD----
#include <LiquidCrystal.h> // biblioteca Liquid Crystal
LiquidCrystal lcd(16, 17, 5, 18, 19, 21);

//----CONEXÃO----
#define INTERVAL 1000
#define SSID ""
#define PASSWORD ""
#define MQTT_SERVER "quickstart.messaging.internetofthings.ibmcloud.com"
#define TOPIC_NAME "iot-2/evt/status/fmt/json"

const String QUICK_START = "d:quickstart:arduino:";
const String DEVICE_ID = "";
const String CLIENT_ID =  QUICK_START + DEVICE_ID;

WiFiClient wifiClient;
PubSubClient client(MQTT_SERVER, 1883, wifiClient);
long lastPublishTime = 0;
float Temperatura = 0;
float Umidade = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();
  setupWiFi();
  connectMQTTServer();
  RTCsetup();  
}

void loop() {
  readSensor();
  LCD();
  RTC();
}

//----conexao----
void setupWiFi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(SSID);

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("Conectando...");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void connectMQTTServer() {
  Serial.println("Connecting to MQTT Server...");
  if (client.connect(CLIENT_ID.c_str())) {
    Serial.println("connected");
  } else {
        Serial.print("error = ");
    Serial.println(client.state());
  }
}
String createJsonString() {
  String data = "{";
    data+= "\"d\": {";
      data+="\"Temperatura\":";
      data+=String(Temperatura);
      data+=",";
      data+="\"Umidade\":";
      data+=String(Umidade);
    data+="}";
  data+="}";
  return data;
}
//----dht11----
void readSensor(){
  float value;
  value = dht.readTemperature();
  if(!isnan(value)){    
    Temperatura = value;
  }
    value = dht.readHumidity();
  
  if(!isnan(value)){
        Umidade = value;
  }
}
//----lcd
 void LCD(){  
  lcd.setCursor(0, 0);
    if (isnan(Temp)){
     lcd.println("Erro na leitura da Temperatura!");
    }
    else 
    {  
      lcd.print("Temp: "); // imprime a Temperatura
      lcd.print(Temp);
      lcd.println(" *C  ");
    }
  lcd.setCursor(1, 1);
  if (isnan(Umi)){
    lcd.println("Erro na leitura da Umidade!");
  }
  else
  {
    lcd.print("Umi: "); // imprime a Umidade
    lcd.print(Umi);
    lcd.println(" %   ");
  }
 }
 //----rtc
 
void RTCsetup(){
  Wire.begin(33, 32);
  Serial.begin(9600);
  if(rtc.begin())
  {
    rtc.adjust(DateTime(ano, mes, data, hora, minuto, segundo));
  }
  else Serial.print("Erro ao iniciar!");
  }
  
void RTC() {
  // put your main code here, to run repeatedly:
  
  DateTime now = rtc.now();
  segundo = now.second();
  minuto = now.minute();
  hora = now.hour();
  dia = diasDaSemana[now.dayOfTheWeek()];
  data = now.day();
  mes = now.month();
  ano = now.year();

  Serial.print(ano);
  Serial.print('/');
  Serial.print(mes);
  Serial.print('/');
  Serial.print(data);
  Serial.print(" (");
  Serial.print(dia);
  Serial.print(") ");
  Serial.print(hora);
  Serial.print(':');
  Serial.print(minuto);
  Serial.print(':');
  Serial.print(segundo);
  Serial.println();
  delay(1000);
}

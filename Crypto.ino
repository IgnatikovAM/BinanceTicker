#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServerSecureBearSSL.h>
#include <Wire.h>
#include "SSD1306Wire.h"     //https://github.com/ThingPulse/esp8266-oled-ssd1306 use version 4.1.0, 4.2.0 doesn't work for me
#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson
#include "images.h"
#include "Adafruit_NeoPixel.h"

String cryptos[] = {"BTCUSDT","ETHUSDT","SOLUSDT","ATOMUSDT","BUSDUSDT","C98USDT","ADAUSDT","MATICUSDT","XRPUSDT","ATAUSDT","DOGEUSDT"};
String cryptosShow[] = {"BTC","ETH","SOL","ATOM","BUSD","C98","ADA","MATIC","XRP","ATA","DOGE"};
char ssid[] = "   ";
char password[] = "    ";
#define ARDUINOJSON_USE_LONG_LONG 1

unsigned long screenChangeDelay = 10000;
WiFiClientSecure client;

SSD1306Wire  display(0x3c, 5, 4);
SSD1306Wire  display2(0x3d, 5, 4);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(10, 14, NEO_GRB + NEO_KHZ800);

unsigned long screenChangeDue;

String ipAddressString;
void setup() {

 Serial.begin(250000);
  
  strip.begin();
  strip.show(); 

strip.setPixelColor(0, strip.Color(33, 33, 33)); // R=, G=0, B=0 — цвет светодиода
strip.show();




  display.init();
  display.setI2cAutoInit(true);
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawXbm(0, 0, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
  display.display();


  display2.init();
  display2.setI2cAutoInit(true);
  display2.flipScreenVertically();
  display2.setTextAlignment(TEXT_ALIGN_CENTER);
  display2.setFont(ArialMT_Plain_10);
  display2.drawString(64, 0, F("Crypto Price"));
  display2.setFont(ArialMT_Plain_16);
  display2.drawString(64, 18, F("by. IgnatikovAM"));
  display2.setFont(ArialMT_Plain_10);
  display2.drawString(64, 48, F("Connecting Binance.com"));
  display2.display();

  

  strip.clear();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  //Serial.print("Connecting Wifi: ");
  //Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
 //Serial.println("");
 //Serial.println("WiFi connected");
 //Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
 //Serial.println(ip);
  ipAddressString = ip.toString();
}
int amountOfCryptos = 11;
int i = 0;
void loop() { unsigned long timeNow = millis();
if (timeNow > screenChangeDue && i <= amountOfCryptos && WiFi.status() == WL_CONNECTED)  {


  
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setInsecure();
    HTTPClient http;
    client->setFingerprint("5B:5F:CA:EA:D0:43:FC:52:2F:D9:E2:EC:A0:6C:A8:57:70:DB:58:F7");
String url=("https://api.binance.com/api/v3/ticker/24hr?symbol=");
  http.begin(*client,url + cryptos[i]);
  int httpCode = http.GET();
                                                             
//Serial.println(http.getString());
//Serial.println("**************");
StaticJsonDocument<1024> doc;

DeserializationError error = deserializeJson(doc, http.getString());

if (error) {
//  Serial.print(F("deserializeJson() failed: "));
 // Serial.println(error.f_str());
  WiFi.reconnect();
// WiFi.setAutoConnect();
}



/*

const char* symbol = doc["symbol"]; // "ETHRUB"
const char* priceChange = doc["priceChange"]; // "-2330.10000000"
const char* priceChangePercent = doc["priceChangePercent"]; // "-0.807"
const char* weightedAvgPrice = doc["weightedAvgPrice"]; // "288256.52701258"
const char* prevClosePrice = doc["prevClosePrice"]; // "289177.30000000"
const char* lastPrice = doc["lastPrice"]; // "286583.00000000"
const char* lastQty = doc["lastQty"]; // "0.30840000"
const char* bidPrice = doc["bidPrice"]; // "286332.10000000"
const char* bidQty = doc["bidQty"]; // "0.10530000"
const char* askPrice = doc["askPrice"]; // "286655.30000000"
const char* askQty = doc["askQty"]; // "0.00130000"
const char* openPrice = doc["openPrice"]; // "288913.10000000"
const char* highPrice = doc["highPrice"]; // "294094.80000000"
const char* lowPrice = doc["lowPrice"]; // "283000.00000000"
const char* volume = doc["volume"]; // "820.79466000"
const char* quoteVolume = doc["quoteVolume"]; // "236599418.08207500"
long long openTime = doc["openTime"]; // 1630668512973
long long closeTime = doc["closeTime"]; // 1630754912973
long firstId = doc["firstId"]; // 2222871
long lastId = doc["lastId"]; // 2227799
int count = doc["count"]; // 4929

*/

//const char* symbol = doc["symbol"];
float priceChange = doc["priceChange"];
float priceChangePercent = doc["priceChangePercent"]; 
//float weightedAvgPrice = doc["weightedAvgPrice"]; 
//float prevClosePrice = doc["prevClosePrice"]; 
//float lastPrice = doc["lastPrice"]; 
//float lastQty = doc["lastQty"]; 
float bidPrice = doc["bidPrice"]; 
//float bidQty = doc["bidQty"];
//float askPrice = doc["askPrice"]; 
//float askQty = doc["askQty"]; 
//float openPrice = doc["openPrice"];
float highPrice = doc["highPrice"]; 
float lowPrice = doc["lowPrice"]; 
float volume = doc["volume"];
float quoteVolume = doc["quoteVolume"];
double openTime = doc["openTime"]; 
double closeTime = doc["closeTime"]; 
//long firstId = doc["firstId"];
//long lastId = doc["lastId"];
//int count = doc["count"];





//дисплей 1
display.clear();
display.setFont(ArialMT_Plain_16);
display.drawString(64, 0, (cryptosShow[i]));
display.setFont(ArialMT_Plain_16);
display.drawString(64, 16, String(bidPrice));
display.setFont(ArialMT_Plain_16);
display.drawString(64, 32, String(priceChangePercent) + "%");
display.setFont(ArialMT_Plain_16);
display.drawString(64, 48, String(priceChange));
display.display();

//дисплей 2

display2.clear();
display2.setTextAlignment(TEXT_ALIGN_CENTER);
display2.setFont(ArialMT_Plain_16);
display2.drawString(64, 0, String(highPrice));
display2.setFont(ArialMT_Plain_16);
display2.drawString(64, 16, String(lowPrice));
display2.setFont(ArialMT_Plain_16);

if(1000000000<=volume){
float volB = (volume)/1000000000;
display2.drawString(64,32,String(volB)+"B");
display2.setFont(ArialMT_Plain_16);
}else if (1000000<=volume){
float volM = (volume)/1000000;
display2.drawString(64,32,String(volM)+"M");
display2.setFont(ArialMT_Plain_16);
}else{
display2.drawString(64,32,String(volume));
display2.setFont(ArialMT_Plain_16);
}

if(1000000000<=quoteVolume){
float quoteVolB = (quoteVolume)/1000000000;
display2.drawString(64,48,String(quoteVolB)+"B");
display2.display();
}else if(1000000<=quoteVolume){
float quoteVolM = (quoteVolume)/1000000;
display2.drawString(64,48,String(quoteVolM)+"M");
display2.display();
}
else{
display2.drawString(64,48,String(quoteVolume));
display2.display();
}










if (0<priceChangePercent) 
    {
     strip.clear();
     strip.setPixelColor(0, strip.Color( 0, 100, 0)); // R=255, G=0, B=0 — цвет светодиода
     strip.show();
 }
    
    if (0 >priceChangePercent) 
    {
    strip.clear();
    strip.setPixelColor(0, strip.Color( 100 , 0, 0)); 
    strip.show();
   }
    
if (0==priceChangePercent)
{   
    strip.clear();
    strip.setPixelColor(0, strip.Color(33, 33, 33));
    strip.show();
}


   http.end();
WiFi.reconnect();
i++;

if (i == amountOfCryptos)

i = 0;

screenChangeDue = timeNow + screenChangeDelay;

 }
}




#include "ESP8266WiFi.h"
#include "WiFiClientSecure.h"
#include "ArduinoJson.h"


#define  DELAYTIME  300 

#define TOTAL_COINS 6

const char* ssid = "  ";
const char* password = "  ";

const char* binanceHost = "api.binance.com";
const char* binanceFingerprint = "5B 5F CA EA D0 43 FC 52 2F D9 E2 EC A0 6C A8 57 70 DB 58 F7";


struct Coin {
  String url;
  String ticker; 
  bool isCoinbaseCoin;
};

struct Coin cryptoCoins[TOTAL_COINS]; 


void connectToWIFI() {
    Serial.println();
    Serial.print("connecting to ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

}


   JsonObject& getJsonObject(String url, bool isCoinbaseCoin) {
    const size_t capacity = (isCoinbaseCoin) ? JSON_OBJECT_SIZE(252) : JSON_OBJECT_SIZE(74);
    DynamicJsonBuffer jsonBuffer(capacity);
    
    WiFiClientSecure client;
    client.setTimeout(10000);
    
   const char* host = (isCoinbaseCoin) ? binanceHost : binanceHost;
   const char* fingerprint = (isCoinbaseCoin) ? binanceFingerprint : binanceFingerprint;
    




 //   Serial.println(host);
    //Serial.printf("Using fingerprint '%s'\n", fingerprint);
    client.setFingerprint(fingerprint);
    
    if (!client.connect(host, 443)) {
        Serial.println("connection failed");
        
        // // стоп, если соединение не удалось,возврат 
       

         return jsonBuffer.parseObject(client);
    }
    //Serial.println(F("Connected!"));
    
    // отправка HTTP запроса
    String httpEnding = (isCoinbaseCoin) ? " HTTP/1.1\r\n" : " HTTP/1.0\r\n";
    client.print(String("GET ") + url + httpEnding +
                 "Host: " + host + "\r\n" +
                 "User-Agent: BuildFailureDetectorESP8266\r\n" +
                 "Connection: close\r\n\r\n");
   // Serial.println("request sent");
    
    // проверка HTTP статуса 
    char status[32] = {0};
    client.readBytesUntil('\r', status, sizeof(status));
    if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
        //Serial.print(F("Unexpected response: "));
      //  Serial.println(status);
        return jsonBuffer.parseObject(client);
    }
    
    // Пропук заголовка HTTP
    char endOfHeaders[] = "\r\n\r\n";
    if (!client.find(endOfHeaders)) {
        //Serial.println(F("Invalid response"));
        
    }
    
    // Parse JSON 
    JsonObject& root = jsonBuffer.parseObject(client);
    if (!root.success()) {
        Serial.println(F("Parsing failed!"));
        
    }
    
    // Откл
    client.stop();
     jsonBuffer.clear();
    return root;
}


void getCoinPrice(String url, String cryptoName, bool isCoinbaseCoin) {
    JsonObject& root = getJsonObject(url, isCoinbaseCoin);
    Serial.println("==========");
  //  Serial.println(F("Response:"));
    Serial.print("Symbol: ");
    Serial.println(root["symbol"].as<char*>());
    Serial.print("Price: ");
    Serial.println(root["price"].as<char*>());
}

void configureCoins() { 
  Coin bitcoin = { .url = "/api/v1/ticker/price?symbol=BTCUSDT", .ticker = "BTCUSDT", .isCoinbaseCoin = false };
  Coin ethereum = { .url = "/api/v1/ticker/price?symbol=ETHUSDT", .ticker = "ETHUSDT", .isCoinbaseCoin = false }; 
  Coin c98 = { .url = "/api/v1/ticker/price?symbol=C98USDT", .ticker = "C98USDT", .isCoinbaseCoin = false };
  Coin ada = { .url = "/api/v1/ticker/price?symbol=ADAUSDT", .ticker = "ADAUSDT", .isCoinbaseCoin = false };
  Coin ata = { .url = "/api/v1/ticker/price?symbol=ATAUSDT", .ticker = "ATAUSDT", .isCoinbaseCoin = false };
  Coin matic = { .url = "/api/v1/ticker/price?symbol=MATICUSDT", .ticker = "MATICUSDT", .isCoinbaseCoin = false }; 
  
  cryptoCoins[0] =  bitcoin;
  cryptoCoins[1] =  ethereum;
  cryptoCoins[2] =  c98;
  cryptoCoins[3] =  ada;
  cryptoCoins[4] = ata;
  cryptoCoins[5] =  matic;

}

void getAllCoinPrices() {
  Coin currentCoin;
  for(int index=0; index < TOTAL_COINS; index++) {
    currentCoin = cryptoCoins[index];
    getCoinPrice(currentCoin.url, currentCoin.ticker, currentCoin.isCoinbaseCoin);
  }
}

void setup() {
    
    Serial.begin(115200);
    connectToWIFI();
    configureCoins();
}

void loop() {
    getAllCoinPrices();
   delay(5000);
}


#include "ESP8266WiFi.h"
#include <ESP8266WebServer.h>
#include "html_header.h"
#include <Adafruit_NeoPixel.h>

// порт сервера 
ESP8266WebServer server(80);

#define NUM_PIX 32//Количество светодиодов 
#define PIN 5 //пин esp
const char* ssid     = "";//имя сети 
const char* password = "";//пароль 
unsigned char red,green,blue;
String form;

//Количество пикселей выаод
 // NEO_KHZ800,  WS2812
 // NEO_GRB, WS2812  в порядке GRB
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIX, PIN, NEO_GRBW + NEO_KHZ800);

//  root веб со ссылкой на страницу настроек
void handle_root() {
  server.send(200, "text/html", "<html>WS2812 WiFi <a href='./out'>Control</a></html>");
  delay(100);
}

// root отобразить колесо цвета и выбор цвета
void handle_outputs() {
  // Строки для хранения вывода клиента
  String RMsg;
  String GMsg;
  String BMsg;

  // Анализировать вывод клиента
  RMsg=server.arg("R");
  GMsg=server.arg("G"); 
  BMsg=server.arg("B");

  // Преобразование в число для перехода в библиотеку Neopixel
  red=RMsg.toInt();
  green=GMsg.toInt(); 
  blue=BMsg.toInt();  

  // Обновление формы и отправка клиенту
  form = "<html>";
  form = form + html_header;
  form = form + "</html>";
  // отправка формы
  server.send(200, "text/html", form);
}

// Инициализация Wi-Fi, веб-сервер 
void setup() {

  Serial.begin(115200);
  // подключение wifi 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // IP-адрес
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(PIN, OUTPUT);
  // Настройка по умолчанию тусклый белый
  red=255;
  green=103;
  blue=23;
  
  // дескрипторы к веб-серверу
   // Базовая страница
  server.on("/", handle_root);
  // страница настроек 
  server.on("/out", handle_outputs);
  // запуск веб сервера 
  server.begin();
  // запуск пикселей 
  pixels.begin();
  pixels.setBrightness(255);
}

void loop() {
  int i;
  
  // Обработка запросов клиентов
  server.handleClient(); 

  // Обновление светодиодной строки
  for(i=0;i<NUM_PIX;i++){
    // RGB
    pixels.setPixelColor(i, pixels.Color(red,green,blue));
    pixels.show();
  }
  delay(500);    // задержка 
}

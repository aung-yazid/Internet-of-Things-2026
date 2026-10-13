#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include "html.h"  // Import file HTML dari header

// true = ESP32 bikin WiFi sendiri (Access Point), false = konek ke WiFi (Station)
const bool apMode = false;

// Mode Station
const char* ssid = "Masukkan Nama WiFi";
const char* password = "Masukkan Password WiFi";

// Mode Access Point
const char* ap_ssid = "ESP32-P3";
const char* ap_password = "12345678";  // minimal 8 karakter
IPAddress ap_ip(192, 168, 3, 1);
IPAddress ap_mask(255, 255, 255, 0);

const int dhtPin = 15;
const int ledPin = 4;
const int sdaPin = 7;
const int sclPin = 17;

DHT dht(dhtPin, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2); 
WebServer server(80);

bool ledNyala = false;
float suhu = NAN;
float lembap = NAN;
unsigned long waktuBaca = 0;
String alamatIP = "";

void handleRoot() {
  server.send_P(200, "text/html", MAIN_page);  // Ambil HTML dari PROGMEM
}

// Dipanggil browser tiap 2 detik, balasannya JSON
void handleData() {
  String json = "{";
  if (isnan(suhu) || isnan(lembap)) {
    json += "\"error\":true,";
  } else {
    json += "\"temp\":" + String(suhu, 1) + ",";
    json += "\"hum\":" + String(lembap, 0) + ",";
  }
  json += "\"led\":" + String(ledNyala ? "true" : "false") + "}";
  server.send(200, "application/json", json);
}

void handleLEDOn() {
  ledNyala = true;
  digitalWrite(ledPin, HIGH);
  Serial.println("LED menyala");
  server.send(200, "application/json", "{\"led\":true}");
}

void handleLEDOff() {
  ledNyala = false;
  digitalWrite(ledPin, LOW);
  Serial.println("LED mati");
  server.send(200, "application/json", "{\"led\":false}");
}

void tampilLCD() {
  lcd.setCursor(0, 0);
  lcd.print(alamatIP);
  lcd.print("                ");

  lcd.setCursor(0, 1);
  if (isnan(suhu) || isnan(lembap)) {
    lcd.print("Sensor error    ");
  } else {
    lcd.print("T:");
    lcd.print(suhu, 1);
    lcd.print("C H:");
    lcd.print(lembap, 0);
    lcd.print("%   ");
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.begin(115200);
  Wire.begin(sdaPin, sclPin);
  lcd.init();
  lcd.backlight();
  dht.begin();

  lcd.setCursor(0, 0);
  lcd.print("Menghubungkan...");

  if (apMode) {
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(ap_ip, ap_ip, ap_mask);
    WiFi.softAP(ap_ssid, ap_password);
    alamatIP = WiFi.softAPIP().toString();
  } else {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    alamatIP = WiFi.localIP().toString();
  }

  Serial.print("IP address: ");
  Serial.println(alamatIP);

  // Setup routes
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.on("/led-on", handleLEDOn);
  server.on("/led-off", handleLEDOff);

  server.begin();
  Serial.println("HTTP server started");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(alamatIP);
  lcd.setCursor(0, 1);
  lcd.print("Buka di browser");
  delay(3000);
  lcd.clear();
}

void loop() {
  server.handleClient();

  // Baca DHT11 tiap 2 detik tanpa delay() supaya web tetap responsif
  if (millis() - waktuBaca >= 2000) {
    waktuBaca = millis();
    suhu = dht.readTemperature();
    lembap = dht.readHumidity();
    tampilLCD();
  }
}
#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

// Pengaturan WiFi
const char* WIFI_SSID = "Redmi Note 14s";
const char* WIFI_PASS = "aung1234";

// Tahap 1: false (lihat error CORS). Tahap 2: true, lalu upload ulang.
const bool ENABLE_CORS = false;

// Pin Definition
const int LED_PIN = 4;
const int DHT_PIN = 15;
const int DHT_TYPE = DHT11;

DHT dht(DHT_PIN, DHT_TYPE);
WebServer server(80);

float lastTemp = NAN;
float lastHum  = NAN;
bool  ledState = false;
unsigned long lastRead = 0;

// Header CORS: izin untuk browser agar halaman dari origin lain boleh membaca respons
void addCors() {
  if (ENABLE_CORS) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
  }
}

void sendJson(const String& body) {
  addCors();
  server.send(200, "application/json", body);
}

void handleSensor() {
  Serial.println("[REQ] GET /api/sensor");
  String json = "{";
  json += "\"temp\":" + (isnan(lastTemp) ? String("null") : String(lastTemp, 1)) + ",";
  json += "\"hum\":"  + (isnan(lastHum)  ? String("null") : String(lastHum, 0))  + ",";
  json += "\"led\":"  + String(ledState ? 1 : 0);
  json += "}";
  sendJson(json);
}

void handleLed() {
  if (server.hasArg("state")) {
    ledState = (server.arg("state") == "1");
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  }
  Serial.printf("[REQ] GET /api/led -> LED %s\n", ledState ? "ON" : "OFF");
  sendJson("{\"led\":" + String(ledState ? 1 : 0) + "}");
}

void handleNotFound() {
  addCors();
  server.send(404, "application/json", "{\"error\":\"endpoint tidak ditemukan\"}");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  dht.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("");
  Serial.print("IP ESP32  : ");
  Serial.println(WiFi.localIP());          // <-- salin IP ini ke config.js
  Serial.print("CORS aktif: ");
  Serial.println(ENABLE_CORS ? "YA" : "TIDAK");
  Serial.println("");

  server.on("/api/sensor", HTTP_GET, handleSensor);
  server.on("/api/led", HTTP_GET, handleLed);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {
  server.handleClient();

  // DHT11 hanya boleh dibaca tiap >= 2 detik
  if (millis() - lastRead >= 2000) {
    lastRead = millis();
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (!isnan(t)) lastTemp = t;
    if (!isnan(h)) lastHum  = h;
  }
}

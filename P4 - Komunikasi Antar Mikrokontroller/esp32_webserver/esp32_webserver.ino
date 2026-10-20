#include <WiFi.h>
#include <WebServer.h>
#include "index_html.h"

// Ganti sesuai jaringan 
const char* WIFI_SSID = "Redmi Note 14s";
const char* WIFI_PASS = "aung1234";

// Hotspot cadangan bila WiFi di atas gagal tersambung
const char* AP_SSID = "IoT-Praktikum";
const char* AP_PASS = "12345678";      // minimal 8 karakter

// Pin komunikasi ke Uno 
const int UNO_RX = 18;   // menerima dari Uno pin 11 (lewat pembagi tegangan)
const int UNO_TX = 17;   // mengirim ke Uno pin 10

const unsigned long UNO_TIMEOUT = 6000;  // Uno dianggap mati jika diam lebih dari 6 detik

WebServer server(80);

// Data terakhir dari Uno
float suhu = 0, lembap = 0, ambang = 30;
int ledState = 0, sensorOk = 0;
char modeName[8] = "MANUAL";
unsigned long terakhirDiterima = 0;

String rxBuf;

void kirimKeUno(const String& perintah) {
  Serial1.print(perintah);
  Serial1.print('\n');
  Serial.println("-> Uno: " + perintah);
}

void parseBarisDariUno(String baris) {
  baris.trim();
  if (!baris.startsWith("DATA,")) return;

  float t, h, a;
  int led, ok;
  char mode[8];
  int n = sscanf(baris.c_str(), "DATA,%f,%f,%d,%7[^,],%f,%d", &t, &h, &led, mode, &a, &ok);
  if (n != 6) return;   // baris rusak, abaikan

  suhu = t;  lembap = h;  ledState = led;  ambang = a;  sensorOk = ok;
  strncpy(modeName, mode, sizeof(modeName));
  modeName[sizeof(modeName) - 1] = '\0';
  terakhirDiterima = millis();

  Serial.println("<- Uno: " + baris);
}

void bacaUno() {
  while (Serial1.available()) {
    char c = Serial1.read();
    if (c == '\n') {
      parseBarisDariUno(rxBuf);
      rxBuf = "";
    } else if (c != '\r' && rxBuf.length() < 80) {
      rxBuf += c;
    }
  }
}

// Handler web
void handleRoot() {
  server.send_P(200, "text/html; charset=utf-8", INDEX_HTML);
}

void handleStatus() {
  bool unoHidup = terakhirDiterima != 0 && (millis() - terakhirDiterima) < UNO_TIMEOUT;

  char json[200];
  snprintf(json, sizeof(json),
           "{\"temp\":%.1f,\"hum\":%.0f,\"led\":%d,\"mode\":\"%s\",\"th\":%.1f,\"ok\":%d,\"uno\":%s}",
           suhu, lembap, ledState, modeName, ambang, sensorOk, unoHidup ? "true" : "false");

  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "application/json", json);
}

void handleSet() {
  if (server.hasArg("mode")) {
    String m = server.arg("mode");
    if (m == "manual")    kirimKeUno("MODE:MANUAL");
    else if (m == "auto") kirimKeUno("MODE:AUTO");
  }
  if (server.hasArg("led")) {
    String v = server.arg("led");
    if (v == "on")        kirimKeUno("LED:ON");
    else if (v == "off")  kirimKeUno("LED:OFF");
  }
  if (server.hasArg("th")) {
    float v = server.arg("th").toFloat();
    if (v >= 0 && v <= 50) kirimKeUno("TH:" + String(v, 1));
  }
  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "text/plain", "OK");
}

void sambungWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Menyambung ke WiFi");

  unsigned long mulai = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - mulai < 15000) {
    delay(500);
    Serial.print('.');
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("\nTersambung. Buka di browser: http://");
    Serial.println(WiFi.localIP());
  } else {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASS);
    Serial.print("\nWiFi gagal. Hotspot aktif: ");
    Serial.print(AP_SSID);
    Serial.print(", buka http://");
    Serial.println(WiFi.softAPIP());
  }
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, UNO_RX, UNO_TX);

  sambungWiFi();

  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.on("/set", handleSet);
  server.onNotFound([]() { server.send(404, "text/plain", "Halaman tidak ditemukan"); });
  server.begin();
}

void loop() {
  server.handleClient();
  bacaUno();
}

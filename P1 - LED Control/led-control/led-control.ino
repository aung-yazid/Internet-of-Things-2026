int ledKuning = 1;

void setup() {
  Serial.begin(9600);
  pinMode(ledKuning, OUTPUT);
}

void loop() {
  digitalWrite(ledKuning, HIGH);
  Serial.println("LED Kuning menyala");
  delay(1000);
  digitalWrite(ledKuning, LOW);
  Serial.println("LED Kuning mati");
  delay(1000);
}
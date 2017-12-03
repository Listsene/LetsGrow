int soilSensorPin = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print(analogRead(soilSensorPin));
  Serial.println();
  delay(2000);
}

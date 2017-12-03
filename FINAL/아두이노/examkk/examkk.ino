void setup() {
  Serial.begin(500000);
}

void loop() {
  int incomingValue;
  if (Serial.available() > 0) {
    incomingValue = Serial.read();
     Serial.println(incomingValue);
  }
}

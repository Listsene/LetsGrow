int value = 1000;
void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print(value);
  value++;
  delay(1000);
}

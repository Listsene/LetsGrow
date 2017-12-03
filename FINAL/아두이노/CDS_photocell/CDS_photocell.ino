int cds = A4;
void setup(){
  Serial.begin(9600);
  pinMode(cds,INPUT);
}

void loop(){
  int cds = analogRead(A0);
  Serial.println(cds);
  delay(500);
}


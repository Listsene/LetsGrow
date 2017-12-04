
void setup() { 
  //start serial connection 
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(13, OUTPUT); 
} 
void loop(){  
  //read the switch value into a variable  
  int sensorVal = digitalRead(7);  //print out the value of the liquid level  
  Serial.println(sensorVal);  
  if (sensorVal == HIGH) {    digitalWrite(13, LOW);      } 
  else {    digitalWrite(13, HIGH);  } 
  delay(1500);
}

/*
int cds = A4;
int LED = 8;
 
void setup() {
  Serial.begin(9600);
  pinMode(cds,INPUT);
  pinMode(2, INPUT);
  pinMode(13, OUTPUT); 
}
 
void loop() {
  int val = analogRead(cds);
  Serial.print("cds_sensor: ");
  Serial.println(val);
 
  if(val>900){
    digitalWrite(LED,HIGH);
    Serial.println("LED ON");
  }
  else{
    digitalWrite(LED,LOW);
    Serial.println("LED OFF");
  }
  delay(1500);
}*/

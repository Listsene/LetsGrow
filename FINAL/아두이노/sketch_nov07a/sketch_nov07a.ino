#include<Servo.h>

Servo servo;

void setup()
{
  servo.attach(9);
  Serial.begin(115200);
}

void loop()
{
  int iValue0 = analogRead(A0);
  Serial.print("A0:");
  Serial.print(iValue0);
  Serial.println();
  if(iValue0>950){
    servo.write(0);
    delay(1000);
    servo.write(100);
    delay(1000);
  }
  delay(500);
}


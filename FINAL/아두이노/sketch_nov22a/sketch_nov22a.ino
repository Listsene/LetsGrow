#include<DHT11.h>
#include "HX711.h"  //You must have this library in your arduino library folder

#define DOUT  3
#define CLK  2

#define SensorPin A0            //pH meter Analog output to Arduino Analog Input 0
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10], temp;

const int soilSensorPin1 = A1;
const int soilSensorPin2 = A2;
const int soilSensorPin3 = A3;

int pin = 4;
DHT11 dht11(pin);

HX711 scale(DOUT, CLK);

int cds = A4;

float calibration_factor = -340000; //-106600 worked for my 40Kg max scale setup

void setup() {
  Serial.begin(500000);
  pinMode(2, INPUT);
  pinMode(13, OUTPUT);
  scale.set_scale(calibration_factor);
  scale.tare();
}

void loop() {
  int incomingValue = 0;
  String soil1, soil2, soil3;
  float temp = 0, humi = 0;
  String tempAndHumi;
  static float pHValue;
  String ph;
  String loadCell;
  String cdsValue;
  int err;
  if (Serial.available() > 0) {
    incomingValue = Serial.read();
  }
  if (incomingValue == 49) {
    soil1 = 1;
    soil1 += ':';
    soil1 += analogRead(soilSensorPin1);
    Serial.println(soil1);
  } else if (incomingValue == 50) {
    soil2 = 2;
    soil2 += ':';
    soil2 += analogRead(soilSensorPin2);
    Serial.println(soil2);
  }  else if (incomingValue == 51 ) {
    soil3 = 3;
    soil3 += ':';
    soil3 += analogRead(soilSensorPin3);
    Serial.println(soil3);
  }  else if (incomingValue == 52) {
    tempAndHumi = 4;
    tempAndHumi += ':';
    if ((err = dht11.read(humi, temp)) == 0) {
      for (int i = 0 ; i < 2 ; i++) {
        if (i == 0) {
          tempAndHumi += temp;
          tempAndHumi += '/';
        }
        else if (i == 1) {
          tempAndHumi += humi;
          Serial.println(tempAndHumi);
        }
      }
    }
  } else if (incomingValue == 53) {
    for (int i = 0; i < 10; i++) //Get 10 sample value from the sensor for smooth the value
    {
      buf[i] = analogRead(SensorPin);
      //delay(10);
    }
    for (int i = 0; i < 9; i++) //sort the analog from small to large
    {
      for (int j = i + 1; j < 10; j++)
      {
        if (buf[i] > buf[j])
        {
          temp = buf[i];
          buf[i] = buf[j];
          buf[j] = temp;
        }
      }
    }
    avgValue = 0;
    for (int i = 2; i < 8; i++)               //take the average value of 6 center sample
      avgValue += buf[i];
    float phValue = (float)avgValue * 5.0 / 1024 / 6; //convert the analog into millivolt
    phValue = 3.5 * phValue;                  //convert the millivolt into pH value
    ph = 5;
    ph += ':';
    ph += phValue;
    Serial.println(ph);

  } else if (incomingValue == 54) {
    /*
      loadCell = scale.get_units();
      loadCell += ':';
      Serial.println(scale.get_units(), 3);  //Up to 3 decimal points*/
    Serial.println("loadCell");
  } else if (incomingValue == 55) {
    cdsValue = 7;
    cdsValue += ':';
    int val = analogRead(cds);
    
    if (val > 900) {
      digitalWrite(13, LOW);
      cdsValue += 0;
      Serial.println(cdsValue);
    }
    else {
      digitalWrite(13, HIGH);
      cdsValue += 1;
      Serial.println(cdsValue);
    }
  }
  delay(2000);
  /*
    soil = analogRead(soilSensorPin1);
    soil += ':';
    soil += analogRead(soilSensorPin2);
    Serial.println(soil);
    if ((err = dht11.read(humi, temp)) == 0) {
    for(int i=0 ; i<2 ; i++){
        if(i==0){
          tempAndHumi=temp;
          tempAndHumi+=':';
          Serial.print(tempAndHumi);
        }
        else if(i==1) Serial.println(humi);
    }
    }
    for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
    {
    buf[i]=analogRead(SensorPin);
    //delay(10);
    }
    for(int i=0;i<9;i++)        //sort the analog from small to large
    {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
    }
    avgValue=0;
    for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
    float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
    phValue=3.5*phValue;                      //convert the millivolt into pH value
    Serial.println(phValue,2);
    Serial.print(scale.get_units(), 3);  //Up to 3 decimal points
    delay(2000);
  */
}

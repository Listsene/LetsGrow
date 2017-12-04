#include<DHT11.h>

#define SensorPin A0            //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00            //deviation compensate
#define samplingInterval 20
#define printInterval 3000
#define ArrayLenth  40    //times of collection
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex = 0;

int pin = 2; // dht11 signal이 연결된 아두이노 피넌호

DHT11 dht11(pin);

const int soilSensorPin = A1;

void setup() {
  Serial.begin(9600);
}

void loop() {
  soilSensor();
  phSensor();
  dht11Sensor();
}

double avergearray(int* arr, int number) {
  int i;
  int max, min;
  double avg;
  long amount = 0;
  if (number <= 0) {
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if (number < 5) { //less than 5, calculated directly statistics
    for (i = 0; i < number; i++) {
      amount += arr[i];
    }
    avg = amount / number;
    return avg;
  } else {
    if (arr[0] < arr[1]) {
      min = arr[0]; max = arr[1];
    }
    else {
      min = arr[1]; max = arr[0];
    }
    for (i = 2; i < number; i++) {
      if (arr[i] < min) {
        amount += min;      //arr<min
        min = arr[i];
      } else {
        if (arr[i] > max) {
          amount += max;  //arr>max
          max = arr[i];
        } else {
          amount += arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount / (number - 2);
  }//if
  return avg;
}

void soilSensor() {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  int soilValue;                                                                                                                                                                                                                                                                                                                                                                                                                               
  if (millis() - samplingTime > samplingInterval)
  {
    samplingTime = millis();
  }
  if (millis() - printTime > printInterval)
    //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
    Serial.print("Soil value : ");
      soilValue=analogRead(soilSensorPin);
    Serial.println(soilValue);
    printTime = millis();
  }
}

void phSensor() {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue, voltage;
  if (millis() - samplingTime > samplingInterval)
  {
    pHArray[pHArrayIndex++] = analogRead(SensorPin);
    if (pHArrayIndex == ArrayLenth)pHArrayIndex = 0;
    voltage = avergearray(pHArray, ArrayLenth) * 5.0 / 1024;
    pHValue = 3.5 * voltage + Offset;
    samplingTime = millis();
  }
  if (millis() - printTime > printInterval)
    //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
    Serial.print("Voltage:");
    Serial.print(voltage, 2);
    Serial.print("    pH value: ");
    Serial.println(pHValue, 2);
    printTime = millis();
  }
}

void dht11Sensor() {
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  int err;
  float temp, humi;
  if (millis() - samplingTime > samplingInterval)
  {
    samplingTime = millis();
  }
  if (millis() - printTime > printInterval)
    //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
    if ((err = dht11.read(humi, temp)) == 0) //온도, 습도 읽어와서 표시
    {
      Serial.print("temperature:");
      Serial.print(temp);
      Serial.print(" humidity:");
      Serial.print(humi);
      Serial.println();
    }
    printTime = millis();
  }
}


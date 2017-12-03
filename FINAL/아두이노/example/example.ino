#define SERIAL_BAUDRATE 115200

#define OPC_PIN_MODE         0x01
#define OPC_DIGITAL_READ     0x02
#define OPC_DIGITAL_WRITE    0x03
#define OPC_ANALOG_REFERENCE 0x04
#define OPC_ANALOG_READ      0x05
#define OPC_ANALOG_WRITE     0x06

long pinVal = 0;
int  inpVal = 0;
long outVal = 0;

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
}

void loop() {
  pinVal = 0, inpVal = 0, outVal = 0;
  while (Serial.available() > 0) {
    switch (Serial.read()) {
      case OPC_PIN_MODE: {
          pinMode(Serial.read(), Serial.read());
          break;
        }
      case OPC_DIGITAL_READ: {
          delay(50);
          pinVal = Serial.read();         // pin값을 pinVal에 저장하고,
          inpVal = digitalRead(pinVal);   // 해당 핀에 연결된 값을 읽어서 inpVal에 저장한다.
          outVal = pinVal << 16 | inpVal; // pin값을 상위 2byte로 shift한 후에 inpVal을 하위 2byte에 넣는다.
          Serial.println(outVal);         // 읽어들인 값을 출력하는 형태로 전송한다.
          break;
        }
      case OPC_DIGITAL_WRITE: {
          digitalWrite(Serial.read(), Serial.read());
          break;
        }
      case OPC_ANALOG_REFERENCE: {
          analogReference(Serial.read());
          break;
        }
      case OPC_ANALOG_READ: {
          delay(50);
          pinVal = Serial.read();
          inpVal = analogRead(pinVal);
          outVal = pinVal << 16 | inpVal;
          Serial.println(outVal);
          break;
        }
      case OPC_ANALOG_WRITE: {
          analogWrite(Serial.read(), Serial.read());
          break;
        }
    }
  }
}

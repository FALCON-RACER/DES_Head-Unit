#include <SPI.h>
#include "mcp2515_can.h"

#define sensor 3
const int SPI_CS_PIN = 9;
mcp2515_can CAN(SPI_CS_PIN);

volatile unsigned int pulseCount = 0;
unsigned long currentMillis, previousMillis = 0;
const unsigned long interval = 1000; // 1초 단위 측정 (필요에 따라 조정 가능)

void countPulses() {
  pulseCount++;
}

void setup() {
  Serial.begin(115200);

  // CAN 초기화
  while (CAN_OK != CAN.begin(CAN_500KBPS)) {
    Serial.println("CAN init fail, retry...");
    delay(100);
  }
  Serial.println("CAN init ok!");

  // 센서 핀 설정
  pinMode(sensor, INPUT);
  attachInterrupt(digitalPinToInterrupt(sensor), countPulses, RISING);
}

void loop() {
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();

  const int wheelHoles = 20;
  static unsigned int pulseSnapshot = 0;

  if (currentMillis - pulseSnapshot >= 1000) { // 1초마다 RPM 계산
    noInterrupts();                 // 인터럽트 임시 중지
    unsigned int pulses = pulseCount; 
    pulseCount = 0;                 // 펄스 카운트 초기화
    interrupts();                   // 인터럽트 재개

    float rotations = pulses / (float)wheelHoles;
    float rpm = rotations * 60.0;   // rotations per second (1초 주기)
    
    Serial.print("RPM: ");
    Serial.println(rpm);

    // CAN으로 전송
    byte data[4];
    memcpy(data, &rpm, sizeof(float));
    CAN.sendMsgBuf(0x7, 0, sizeof(float), data);
    Serial.println("CAN BUS sendMsgBuf ok!");

    pulseSnapshot = currentMillis;
  }
}

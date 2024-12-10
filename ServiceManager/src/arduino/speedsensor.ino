#include <SPI.h>
#include <mcp2515.h>
#include <TimerOne.h>

const unsigned int sampleDuration = 100; // 0.1 sec in milliseconds
const unsigned int speedSensorPin = 3;
volatile unsigned int speedPulses = 0;

struct can_frame canMsg;
MCP2515 mcp2515(10);

const float wheelDiameter = 6.7; // cm
const float pi = 3.14159;
const float wheelCircumference = wheelDiameter * pi;
const unsigned int numSlots = 20; // number of slots of the disk used for the speedsensor
const unsigned int scaleFactor = 10000;
const unsigned long scaledDistancePerPulse = (unsigned long)(wheelCircumference / numSlots * scaleFactor);
const unsigned int convertToPerSecFactor = 10;
const unsigned int sizeOfUnsignedLong = sizeof(unsigned long);

void setup() {
    Serial.begin(9600);
    pinMode(speedSensorPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(speedSensorPin), countPulses, RISING);
    SPI.begin();
    mcp2515.reset();
    mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
    mcp2515.setNormalMode();
    Timer1.initialize(sampleDuration * 1000UL); // convert to microsecond
    Timer1.attachInterrupt(calculateAndSendSpeedData);
}

void loop() {
  // it is empty to reduce workload since it is always being called
}

void countPulses() {
    speedPulses++;
}

void calculateAndSendSpeedData() {
  unsigned int currentPulses;
  noInterrupts();
  currentPulses = speedPulses;
  speedPulses = 0;
  interrupts();

  const unsigned long scaledSpeedCmPerS = currentPulses * scaledDistancePerPulse * convertToPerSecFactor;

  canMsg.can_id  = 0x100;
  canMsg.can_dlc = sizeOfUnsignedLong;
  memcpy(canMsg.data, &scaledSpeedCmPerS, sizeOfUnsignedLong);

  mcp2515.sendMessage(&canMsg);
}

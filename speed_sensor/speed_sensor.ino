#include <SPI.h>
#include "mcp2515_can.h"

#define sensor 3
const int SPI_CS_PIN = 9;     // default is 9
mcp2515_can CAN(SPI_CS_PIN);  // Set CS pin


volatile unsigned int pulseCount = 0;
unsigned long currentMillis = 0;
unsigned long lastMillis = 0;
const long interval = 100;
unsigned char data[8];

void setup() {
  SERIAL_PORT_MONITOR.begin(115200);

  // init can bus : baudrate = 500k always keep it same to the Rpi CAN interface
  while (CAN_OK != CAN.begin(CAN_500KBPS)) {
      SERIAL_PORT_MONITOR.println("CAN init fail, retry...");
      delay(100);
  }
  SERIAL_PORT_MONITOR.println("CAN init ok!");

  pinMode(sensor, INPUT);
  attachInterrupt(digitalPinToInterrupt(sensor), countPulses, RISING);
}

void loop() {
  
  currentMillis = millis();

  if (currentMillis - lastMillis >= interval) {

    lastMillis = currentMillis;

    float rpm = getRPM();
    Serial.println(rpm);

    pulseCount = 0;
    memcpy(data, &rpm, sizeof(rpm));
    CAN.sendMsgBuf(0x7, 0, 4, data); // Send the data on the CAN bus

    SERIAL_PORT_MONITOR.println("CAN BUS sendMsgBuf ok!"); // Print a success message
  }
}

void countPulses() {
  pulseCount++;
}

float getRPM() {
  int wheelHoles = 20;                  // Number of holes in the wheel
  float seconds = interval / 1000.0;    // Convert interval from milliseconds to seconds

  float rotations = (float)pulseCount / wheelHoles; // Calculate number of rotations
  float rotationsPerSecond = rotations / seconds;   // Calculate rotations per second
  float rpm = rotationsPerSecond * 60;              // Convert to RPM

  return rpm;
}

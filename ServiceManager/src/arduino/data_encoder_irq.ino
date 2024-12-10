#include <stdint.h>

#define NUM_SLOTS 20
#define DEBOUNCE_TIME 600  // Minimum time between interrupts for debounce filtering (in microseconds)

// Variables for storing timestamps and state
unsigned long lastStartTime = 0;        // Time when the last slot started
unsigned long lastEndTime = 0;          // Time when the last slot ended
unsigned long lastInterruptTime = 0;    // Time when the last interrupt occurred, used for debounce filtering
unsigned long currentTimeInInterrupt = 0; // Time captured during the current interrupt, used for processing the slot

// Logical variables for tracking state and events
uint8_t virtualSlotNumber = 0;          // Virtual slot number, ranges from 0 to NUM_SLOTS-1, represents the current slot position
uint16_t rotationCount = 0;             // Counter for the number of full rotations (every 20 slots)
bool isInSlot = false;                  // Flag indicating whether we are currently inside a slot (true) or not (false)
volatile bool newSlotDetected = false;  // Flag set by the interrupt to indicate a new slot event has been detected

void setup() {
    Serial.begin(115200);  // Initialize Serial communication
    attachInterrupt(digitalPinToInterrupt(3), onSensorTrigger, CHANGE); // Set up interrupt on pin 3
}

void loop() {
    // Check if a new slot event has been detected
    if (newSlotDetected) {
        processSlotEvent(); // Process the detected slot event
        newSlotDetected = false;  // Reset the flag after processing
    }
}

// Interrupt Service Routine (ISR) to handle sensor triggers
void onSensorTrigger() {
    unsigned long currentTime = micros(); // Get the current time in microseconds

    // Debounce filtering: ignore interrupts if too little time has passed since the last interrupt
    if (currentTime - lastInterruptTime < DEBOUNCE_TIME) {
        return;
    }
    lastInterruptTime = currentTime; // Update the time of the last interrupt

    // Capture the current time and set the flag for processing in the main loop
    currentTimeInInterrupt = currentTime;
    newSlotDetected = true;
}

// Function to process the slot event, called from the main loop
void processSlotEvent() {
    if (isInSlot) {
        // End of slot (exiting the slot)
        uint16_t intervalEndStart = uint16_t(currentTimeInInterrupt - lastEndTime); // Time between the end of the last slot and the start of the current slot
        uint16_t slotDuration = uint16_t(currentTimeInInterrupt - lastStartTime);   // Duration of the current slot
        uint16_t intervalStartStart = uint16_t(lastStartTime - lastEndTime);        // Time between the start of the last slot and the start of the current slot

        // Output the data via Serial
        Serial.print(virtualSlotNumber);
// Virtual slot number, ranges from 0 to NUM_SLOTS-1, represents the current slot position
        Serial.print(',');
        Serial.print(slotDuration);
        Serial.print(',');
// Duration of the current slot
        Serial.print(intervalStartStart);
// Time between the end of the last slot and the start of the current slot    
        Serial.print(',');
        Serial.print(intervalEndStart);
// Time between the start of the last slot and the start of the current slot        
        Serial.print(',');
// different
//        Serial.print(slotDuration + intervalStartStart - intervalEndStart);
//        Serial.print(',');
        
       
        Serial.println(rotationCount);

        // Update the virtual slot number
        virtualSlotNumber++;
        if (virtualSlotNumber == NUM_SLOTS) {
            virtualSlotNumber = 0;
            rotationCount++;  // Increment the rotation count only when all slots are completed
        }

        // Mark that we are no longer inside a slot and update the end time
        isInSlot = false;
        lastEndTime = currentTimeInInterrupt;
    } else {
        // Start of slot (entering the slot)
        lastStartTime = currentTimeInInterrupt; // Update the start time of the slot
        isInSlot = true; // We are now inside a slot
    }
}

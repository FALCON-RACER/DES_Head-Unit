#include "canreceiver.hpp"
#include "speed.h"

int main() {
    Speed speed = Speed();
    // Arduino Speed Sender ID: 0x0F6
    CANReceiver canreceiver = CANReceiver("can0");

    float filtered_speed = 0;
    float weight = 0.6;

    while (canreceiver.canRead()) {
        filtered_speed = (1-weight)*filtered_speed + (weight)*canreceiver.getSpeed();
        speed.setSpeed(filtered_speed);
    }
    
}

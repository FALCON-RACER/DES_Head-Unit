#include "PiRacer/PiRacer.hpp"
#include "Gamepad/ShanwanGamepad.hpp"

/**
 * A : DRIVE
 * B : NEUTRAL
 * X : REVERSE
 * Y : PARKING
 */

enum Gear {
    PARKING,
    REVERSE,
    NEUTRAL,
    DRIVE
};

int main() {
    atexit(gpioTerminate);
    
	if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed" << std::endl;
        return 1;
    }

    PiRacer racer;
    ShanWanGamepad gamepad;
    Gear gear = PARKING;

    float steering = 0;
    float throttle = 0;

    while (true) {
        ShanWanGamepadInput input = gamepad.read_data();

        steering = 0;
        throttle = 0;

        if (input.button_a)
            gear = DRIVE;
        else if (input.button_b)
            gear = NEUTRAL;
        else if (input.button_x)
            gear = REVERSE;
        else if (input.button_y)
            gear = PARKING;
        else if (gear == PARKING || gear == NEUTRAL)
            continue;

        steering = input.analog_stick_left.x;
        if (input.analog_stick_right.x <= 0)
            throttle = input.analog_stick_right.x * -0.7;
        if (gear == DRIVE)
            throttle *= -1;

        racer.setSteeringPercent(steering);
        racer.setThrottlePercent(throttle);
    }

    return 0;
}


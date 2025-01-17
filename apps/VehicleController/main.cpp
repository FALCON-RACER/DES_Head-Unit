#include "PiRacer/PiRacer.hpp"
#include "Gamepad/ShanwanGamepad.hpp"
#include <cstdlib>

/**
 * A : DRIVE
 * B : NEUTRAL
 * X : REVERSE
 * Y : PARKING
 */

int main()
{
    if (gpioInitialise() < 0)
    {
        std::cerr << "pigpio initialization failed" << std::endl;
        return 1;
    }
    atexit(gpioTerminate);

    PiRacer racer;
    ShanWanGamepad gamepad;

    while (true)
    {
        ShanWanGamepadInput input = gamepad.read_data();

        float steering = input.analog_stick_left.x;
        float throttle = input.analog_stick_right.y * 0.5; // limit throtle to 50%
        float steering = input.analog_stick_left.x * -1;
        float throttle = input.analog_stick_right.x * 0.7; // limit throtle to 50%


        racer.setSteeringPercent(steering);
        racer.setThrottlePercent(throttle);
    }

    return 0;
}

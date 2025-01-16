#include "PiRacer/PiRacer.hpp"
#include "Gamepad/ShanwanGamepad.hpp"

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

        std::cout << input.button_a << std::endl;

        float steering = input.analog_stick_left.x;
        float throttle = input.analog_stick_right.y * 0.5; // limit throtle to 50%

        std::cout << "Throttle: " << throttle
                  << ", Steering: " << steering
                  << std::endl;

        racer.setSteeringPercent(steering);
        racer.setThrottlePercent(throttle);
    }

    return 0;
}

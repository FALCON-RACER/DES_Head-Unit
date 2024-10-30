#ifndef CANRECEIVER_HPP
#define CANRECEIVER_HPP

#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <cstdint>
#include <string>

// CANReceiver class for handling CAN communication
class CANReceiver {
public:
    CANReceiver(const std::string& interface_name);  // Constructor
    ~CANReceiver();                                  // Destructor

    bool canRead();                                  // Check if CAN frame is available
    float getSpeed();                              // Get speed from the CAN frame

private:
    int socket_fd;                                   // File descriptor for the CAN socket
    struct sockaddr_can addr;                        // CAN address
    struct ifreq ifr;                                // Interface request structure
    struct can_frame frame;                          // CAN frame structure

    float calculateSpeed(const struct can_frame* frame);  // Calculate speed from CAN frame data
    void openSocket(const std::string& interface_name);     // Open and bind CAN socket
};

#endif // CANRECEIVER_HPP

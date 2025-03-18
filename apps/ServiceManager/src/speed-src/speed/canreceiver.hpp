// #ifndef CANRECEIVER_HPP
// #define CANRECEIVER_HPP


// // CANReceiver class for handling CAN communication
// class CANReceiver {
    // public:
    //     CANReceiver(const std::string& interface_name);  // Constructor
    //     ~CANReceiver();                                  // Destructor
    
    //     bool canRead();                                  // Check if CAN frame is available
    //     float getSpeed();                              // Get speed from the CAN frame
    
    // private:
    //     int socket_fd;                                   // File descriptor for the CAN socket
    //     struct sockaddr_can addr;                        // CAN address
    //     struct ifreq ifr;                                // Interface request structure
    //     struct can_frame frame;                          // CAN frame structure
    
    //     float calculateSpeed(const struct can_frame* frame) const ;  // Calculate speed from CAN frame data
    //     void openSocket(const std::string& interface_name);     // Open and bind CAN socket
    // };
    
// #endif // CANRECEIVER_HPP
#ifndef CANRECEIVER_HPP
#define CANRECEIVER_HPP

#include "../../headers.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>

class CANReceiver {
private:
    int socket_fd;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

public:
    CANReceiver(const std::string& interface_name);
    CANReceiver();
    ~CANReceiver();

    bool canRead();
    float getSpeed() const;
    int get();
private:
    float calculateSpeed(const struct can_frame* frame) const;
    void openSocket(const std::string& interface_name);
};

#endif // CANRECEIVER_HPP

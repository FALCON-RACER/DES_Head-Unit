#ifndef CANRECEIVER_HPP
#define CANRECEIVER_HPP

#include <string>
#include <linux/can.h>
#include <linux/can/raw.h>
#include "../../headers.hpp"
class CANReceiver {
private:
    int socket_fd;
    struct sockaddr_can addr;
    struct ifreq ifr;

public:
    CANReceiver(const std::string& interface_name);
    ~CANReceiver();

    bool receive(float& speed);
};

#endif
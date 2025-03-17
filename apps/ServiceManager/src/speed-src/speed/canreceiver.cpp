#include "./canreceiver.hpp"
#include "../../headers.hpp"

// Constructor: Opens and binds the CAN socket to the specified interface
#include "canreceiver.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

CANReceiver::CANReceiver(const std::string& interface_name) {
    if ((socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("Socket");
        throw std::runtime_error("CAN Socket creation failed");
    }

    strcpy(ifr.ifr_name, interface_name.c_str());
    if (ioctl(socket_fd, SIOCGIFINDEX, &ifr) < 0) {
        perror("Ioctl");
        close(socket_fd);
        throw std::runtime_error("CAN Interface not found");
    }

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socket_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0) {
        perror("Bind");
        close(socket_fd);
        throw std::runtime_error("CAN Socket bind failed");
    }

    std::cout << "CAN Socket bound to interface: " << interface_name << std::endl;
}

CANReceiver::~CANReceiver() {
    if(socket_fd >= 0) close(socket_fd);
}

bool CANReceiver::receive(float& speed) {
    struct can_frame frame;
    int nbytes = read(socket_fd, &frame, sizeof(struct can_frame));

    if (nbytes < 0) {
        perror("CAN Read Error");
        return false;
    }

    if (nbytes != sizeof(struct can_frame)) {
        std::cerr << "Incomplete CAN frame received." << std::endl;
        return false;
    }

    // CAN ID 체크 (필요에 따라 특정 ID만 처리)
    // if (frame.can_id != 0x100) {
    //     std::cerr << "Unknown CAN ID: " << frame.can_id << std::endl;
    //     return false;
    // }

    // 데이터 복사 (float 형태로 정확히 처리)
    memcpy(&speed, frame.data, sizeof(float));

    std::cout << "Received Speed: " << speed << " cm/s" << std::endl;
    return true;
}

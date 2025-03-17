#include "./canreceiver.hpp"
#include "../../headers.hpp"

// Constructor: Opens and binds the CAN socket to the specified interface
CANReceiver::CANReceiver(const std::string& interface_name) : socket_fd(-1) {
    openSocket(interface_name);
}

// Destructor: Closes the CAN socket
CANReceiver::~CANReceiver() {
    if (socket_fd != -1) {
        close(socket_fd);
        std::cout << "CAN socket closed." << std::endl;
    }
}

// Opens a CAN socket and binds it to the given interface
void CANReceiver::openSocket(const std::string& interface_name) {
    // Open the CAN_RAW socket

    try {
        socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        if (socket_fd < 0) {
            perror("Error while opening CAN socket");
            throw std::runtime_error("Failed to open CAN socket");
        }

        // Get the interface index for the specified CAN interface
        std::strncpy(ifr.ifr_name, interface_name.c_str(), IFNAMSIZ - 1);
        if (ioctl(socket_fd, SIOCGIFINDEX, &ifr) < 0) {
            perror("Error finding CAN interface");
            close(socket_fd);  // Ensure socket is closed before throwing
            throw std::runtime_error("Failed to find CAN interface");
        }

        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;

        // Bind the socket to the CAN interface
        if (bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("Error in binding CAN socket");
            close(socket_fd);  // Ensure socket is closed before throwing
            throw std::runtime_error("Failed to bind CAN socket");
        }
    } catch (char &e) {
        std::cout << e << std::endl;
    }

    std::cout << "CAN socket bound to interface: " << interface_name << std::endl;
}

// Check if a CAN frame is available (read from socket)
bool CANReceiver::canRead() {
    int nbytes = read(socket_fd, &frame, sizeof(struct can_frame));
    if (nbytes < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return false;  // No data available, not an error
        }
        perror("Error reading CAN frame");
        return false;
    }
    if (nbytes < sizeof(struct can_frame)) {
        std::cerr << "Incomplete CAN frame" << std::endl;
        return false;
    }
    return true;
}

// Extract speed from the received CAN frame
float CANReceiver::getSpeed() {
    return calculateSpeed(&frame);
}

// Calculate speed from the first two bytes of the CAN frame data
float CANReceiver::calculateSpeed(const struct can_frame* frame) {
    // Interpret speed as a signed 16-bit integer
    float speed=0;
    memcpy (&speed, &frame->data, sizeof(float));
    std::cout << "memcpy value : " <<speed << std::endl;

    std::cout << " 0 : "<<frame->data[0] << std::endl;
    std::cout << " 1 : "<<frame->data[1] << std::endl;
    std::cout << " 2 : "<<frame->data[2] << std::endl;
    std::cout << " 3 : "<<frame->data[3] << std::endl;
    std::cout << " 4 : "<<frame->data[4] << std::endl;
    std::cout << " 5 : "<<frame->data[5]<< std::endl;
    std::cout << " 6 : "<<frame->data[6] << std::endl;
    std::cout << " 7 : "<<frame->data[7]<< std::endl;
    float speed = static_cast<float>((frame->data[0] << 8) | frame->data[1]);
    // float speed = 120;
    std::cout << "Speed: " << speed << std::endl << std::flush;
    return speed;
} 
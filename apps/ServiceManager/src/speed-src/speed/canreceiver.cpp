#include "./canreceiver.hpp"
#include "../../headers.hpp"

#include "canreceiver.hpp"

// // Constructor: Opens and binds the CAN socket to the specified interface
CANReceiver::CANReceiver(const std::string& interface_name) : socket_fd(-1) {
    // openSocket(interface_name);
}
CANReceiver::CANReceiver() : socket_fd(-1){}
// Destructor: Closes the CAN socket
CANReceiver::~CANReceiver() {
    if (socket_fd != -1) {
        close(socket_fd);
        std::cout << "CAN socket closed." << std::endl;
    }
}

// // Opens a CAN socket and binds it to the given interface
void CANReceiver::openSocket(const std::string& interface_name) {
    socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (socket_fd < 0) {
        perror("Socket");
        throw std::runtime_error("Failed to open CAN socket");
    }

    std::strncpy(ifr.ifr_name, interface_name.c_str(), IFNAMSIZ - 1);
    if (ioctl(socket_fd, SIOCGIFINDEX, &ifr) < 0) {
        perror("Ioctl");
        close(socket_fd);
        throw std::runtime_error("Failed to get CAN interface index");
    }

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    // 필터 설정 (CAN ID = 0x7만 수신)
    struct can_filter rfilter[1];
    rfilter[0].can_id   = 0x7;
    rfilter[0].can_mask = CAN_SFF_MASK;
    setsockopt(socket_fd, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

    if (bind(socket_fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0) {
        perror("Bind");
        close(socket_fd);
        throw std::runtime_error("Failed to bind CAN socket");
    }

    std::cout << "CAN socket bound to interface: " << interface_name << std::endl;
}

// Check if a CAN frame is available (read from socket)
bool CANReceiver::canRead() {
    int nbytes = read(socket_fd, &frame, sizeof(struct can_frame));

    if (nbytes < 0) {
        perror("CAN read failed");
        return false;
    }

    if (nbytes != sizeof(struct can_frame)) {
        std::cerr << "Incomplete CAN frame received (size = " << nbytes << ")\n";
        return false;
    }

    if(frame.can_id == 0x7 && frame.can_dlc == 4) {
        float rpm = calculateSpeed(&frame);
        std::cout << "Received RPM: " << rpm << std::endl;
        return true;
    } else {
        std::cerr << "Received frame with unexpected CAN ID or DLC." << std::endl;
        return false;
    }
}

// Return speed value from the received CAN frame
float CANReceiver::getSpeed() const {
    return calculateSpeed(&frame);
}

// Calculate speed (RPM) from CAN frame data
float CANReceiver::calculateSpeed(const struct can_frame* frame) const {
    float rpm = 0.0f;
    memcpy(&rpm, frame->data, sizeof(float));
    return rpm;
}
//#################

// // #include "mcp2515_can.h"
// // #include <SPI.h>

// // Constructor: Opens and binds the CAN socket to the specified interface
// CANReceiver::CANReceiver(const std::string& interface_name) : socket_fd(-1) {
//     openSocket(interface_name);
// }

// // Destructor: Closes the CAN socket
// CANReceiver::~CANReceiver() {
//     if (socket_fd != -1) {
//         close(socket_fd);
//         std::cout << "CAN socket closed." << std::endl;
//     }
// }

// // Opens a CAN socket and binds it to the given interface
// void CANReceiver::openSocket(const std::string& interface_name) {
//     // Open the CAN_RAW socket
//     try {
//         socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
//         if (socket_fd < 0) {
//             perror("Error while opening CAN socket");
//             throw std::runtime_error("Failed to open CAN socket");
//         }

//         // Get the interface index for the specified CAN interface
//         std::strncpy(ifr.ifr_name, interface_name.c_str(), IFNAMSIZ - 1);
//         if (ioctl(socket_fd, SIOCGIFINDEX, &ifr) < 0) {
//             perror("Error finding CAN interface");
//             close(socket_fd);  // Ensure socket is closed before throwing
//             throw std::runtime_error("Failed to find CAN interface");
//         }

//         addr.can_family = AF_CAN;
//         addr.can_ifindex = ifr.ifr_ifindex;

//         // Bind the socket to the CAN interface
//         if (bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
//             perror("Error in binding CAN socket");
//             close(socket_fd);  // Ensure socket is closed before throwing
//             throw std::runtime_error("Failed to bind CAN socket");
//         }
//     } catch (char &e) {
//         std::cout << e << std::endl;
//     }

//     std::cout << "CAN socket bound to interface: " << interface_name << std::endl;
// }

// // Check if a CAN frame is available (read from socket)
// bool CANReceiver::canRead() {
//     int nbytes = read(socket_fd, &frame, sizeof(struct can_frame));
//     std::cout << "nbytes : " << nbytes << std::endl;
//     if (nbytes == sizeof(struct can_frame)) {
//         std::cout << "CAN ID: 0x" << std::hex << frame.can_id << std::dec << "\n";
//         std::cout << "DLC: " << static_cast<int>(frame.can_dlc) << "\n";
    
//         std::cout << "CAN Data: ";
//         for (int i = 0; i < frame.can_dlc; i++)
//             printf("%02X ", frame.data[i]);
//         std::cout << std::endl;
    
//         if(frame.can_id == 0x7 && frame.can_dlc == 4) {
//             float rpm = 0.0f;
//             memcpy(&rpm, frame.data, sizeof(float));
//             std::cout << "RPM: " << rpm << "\n";
//         } else {
//             std::cout << "Received frame with unexpected CAN ID or DLC.\n";
//             return false;
//         }
//         return true;
//     } else if (nbytes < 0) {
//         perror("CAN read failed");
//         return false;
//     } else {
//         std::cerr << "Incomplete CAN frame received (size = " << nbytes << ")\n";
//         return false;
//     }
    
//     // if (nbytes == sizeof(struct can_frame)) {
//     //     if(frame.can_id == 0x7 && frame.can_dlc == 4) {
//     //         float rpm;
//     //         memcpy(&rpm, frame.data, sizeof(float));
//     //         std::cout << "received RPM value : " << rpm << std::endl;
//     //     }
//     // }
    
//     // if (nbytes < 0) {
//     //     if (errno == EAGAIN || errno == EWOULDBLOCK) {
//     //         return false;  // No data available, not an error
//     //     }
//     //     perror("Error reading CAN frame");
//     //     return false;
//     // }
//     // if (nbytes < sizeof(struct can_frame)) {
//     //     std::cerr << "Incomplete CAN frame" << std::endl;
//     //     return false;
//     // }
//     // return true;
// }

// // Extract speed from the received CAN frame
// float CANReceiver::getSpeed() {
//     return calculateSpeed(&frame);
// }

// // Calculate speed from the first two bytes of the CAN frame data
// float CANReceiver::calculateSpeed(const struct can_frame* frame) {
//     // Interpret speed as a signed 16-bit integer
//     float speed=0;
//     // memcpy (&speed, frame->data, sizeof(float));
//     memcpy (&speed, frame->data, sizeof(float));
//     // std::cout << "memcpy value : " <<speed << std::endl;
//     // for (int i = 0; i < 16; i++)
//     //     std::cout << i  << " : "<<static_cast<int>(frame->data[i]) << std::endl;

//     // float speed = static_cast<float>((frame->data[0] << 8) | frame->data[1]);
//     // float speed = 120;
//     std::cout << "Speed: " << speed << std::endl << std::flush;
//     return speed;
// } 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>
#include <linux/can/raw.h>

int soc;                   // Socket descriptor
int read_can_port;         // Flag for reading from the CAN port

int open_port(const char *port)
{
    struct ifreq ifr;      // Interface request structure for socket ioctls
    struct sockaddr_can addr;  // Address structure for the CAN socket

    /* open socket */
    soc = socket(PF_CAN, SOCK_RAW, CAN_RAW);  // Open a CAN socket

    if (soc < 0) 
    {
	    printf("error!");
        return (-1);
    }

    addr.can_family = AF_CAN;  // Set the family type for the address to CAN
    strcpy(ifr.ifr_name, port);  // Copy the port name to the ifreq structure

    // Fetch the index of the network interface into the ifreq structure using ioctl
    if (ioctl(soc, SIOCGIFINDEX, &ifr) < 0) 
    {
	    printf("error!");
        return (-1);
    }

    addr.can_ifindex = ifr.ifr_ifindex;  // Get the interface index from the ifreq structure
    fcntl(soc, F_SETFL, O_NONBLOCK);  // Set the socket to non-blocking mode

    // Bind the socket to the CAN interface
    if (bind(soc, (struct sockaddr *)&addr, sizeof(addr)) < 0) 
    {
	    printf("binding error!");
        return (-1);
    }

    return 0;
}

void read_port()
{
    struct can_frame frame;  // Structure to hold the CAN frame data
    frame.can_dlc = 2;      // Set the data length code (DLC) to 2
    int recvbytes = 0;      // Number of bytes received
    read_can_port = 1;      // Flag to continue reading from the CAN port

    while(read_can_port)  // Continue reading as long as the flag is set
    {
        usleep(100000);  // Sleep for 100ms

        struct timeval timeout = {1, 0};  // Set the timeout for the select function to 1 second
        fd_set readSet;  // File descriptor set for the select function
        FD_ZERO(&readSet);  // Clear the set
        FD_SET(soc, &readSet);  // Add the socket descriptor to the set

        // Use the select function to wait for data on the socket or for the timeout
        if (select((soc + 1), &readSet, NULL, NULL, &timeout) >= 0) 
        {
            if (!read_can_port) 
            {
		        printf("error!");
                break;
            }

            // Check if data is available on the socket
            if (FD_ISSET(soc, &readSet)) 
            {
                // Read the data from the socket
                recvbytes = read(soc, &frame, sizeof(struct can_frame));
                if(recvbytes) 
                { 
                   /* ==================== Receive Data ====================*/
                   // Convert the first two bytes of the CAN frame data to the disk rpm
                   int disk_rpm = (frame.data[0] << 8) + frame.data[1];

                   // Calculate the rpm from the disk rpm
                   int rpm = (int)((float)disk_rpm / 2.6);
                   printf("Car RPM: %d\t", rpm);

                   // Calculate the speed from the rpm
                   float speed = (float)rpm * 0.0034;
                   printf("Car Speed: %.3f m/s\n", speed);
                }
            }
        }
    }
}

int close_port()
{
    close(soc);  // Close the socket
    return 0;
}


int CANReceiver::get(void)
{
    // open_port("can0");  // Open the CAN port with the name "can0"
    // read_port();  // Read data from the CAN port
   
    // return 0;

    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

    // CAN 소켓 생성
    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("CAN socket");
        return 1;
    }

    // CAN 인터페이스(can0) 설정
    strcpy(ifr.ifr_name, "can0");
    if (ioctl(s, SIOCGIFINDEX, &ifr) < 0) {
        perror("ioctl");
        close(s);
        return 1;
    }

    // 소켓을 인터페이스에 바인딩
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(s);
        return 1;
    }

    std::cout << "CAN 소켓이 can0 인터페이스에 연결되었습니다.\n";

    // 데이터 수신 루프
    while (true) {
        int nbytes = read(s, &frame, sizeof(struct can_frame));

        if (nbytes < 0) {
            perror("CAN read");
            continue;
        }

        // CAN 데이터 출력
        std::cout << "CAN ID: 0x" << std::hex << frame.can_id << std::dec;
        std::cout << " DLC: " << static_cast<int>(frame.can_dlc) << " Data: ";

        for (int i = 0; i < frame.can_dlc; i++)
            printf("%02X ", frame.data[i]);

        std::cout << std::endl;
    }

    close(s);
    return 0;
}
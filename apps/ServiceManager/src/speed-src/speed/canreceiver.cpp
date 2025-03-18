#include "./canreceiver.hpp"
#include "../../headers.hpp"

#include "canreceiver.hpp"

// // Constructor: Opens and binds the CAN socket to the specified interface

// int CANReceiver::get(void)
// {
//     // open_port("can0");  // Open the CAN port with the name "can0"
//     // read_port();  // Read data from the CAN port
   
//     // return 0;

//     int s;
//     struct sockaddr_can addr;
//     struct ifreq ifr;
//     struct can_frame frame;

//     // CAN 소켓 생성
//     if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
//         perror("CAN socket");   
//         return 1;
//     }

//     // CAN 인터페이스(can0) 설정
//     strcpy(ifr.ifr_name, "can0");
//     if (ioctl(s, SIOCGIFINDEX, &ifr) < 0) {
//         perror("ioctl");
//         close(s);
//         return 1;
//     }

//     // 소켓을 인터페이스에 바인딩
//     addr.can_family = AF_CAN;
//     addr.can_ifindex = ifr.ifr_ifindex;

//     if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
//         perror("bind");
//         close(s);
//         return 1;
//     }

//     std::cout << "CAN 소켓이 can0 인터페이스에 연결되었습니다.\n";

//     // 데이터 수신 루프
//     while (true) {
//         int nbytes = read(s, &frame, sizeof(struct can_frame));

//         if (nbytes < 0) {
//             perror("CAN read");
//             continue;
//         }

//         // CAN 데이터 출력
//         std::cout << "CAN ID: 0x" << std::hex << frame.can_id << std::dec;
//         std::cout << " DLC: " << static_cast<int>(frame.can_dlc) << " Data: ";

//         for (int i = 0; i < frame.can_dlc; i++)
//             printf("%02X ", frame.data[i]);

//         std::cout << std::endl;
//     }

//     close(s);
//     return 0;
// }
int CANReceiver::get(void)
{
    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

    // CAN 소켓 생성
    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("CAN socket");
        return -1;
    }

    // CAN 인터페이스(can0) 설정
    strcpy(ifr.ifr_name, "can0");
    if (ioctl(s, SIOCGIFINDEX, &ifr) < 0) {
        perror("ioctl");
        close(s);
        return -1;
    }

    // 소켓을 인터페이스에 바인딩
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(s);
        return -1;
    }

    std::cout << "CAN 소켓이 can0 인터페이스에 연결되었습니다.\n";

    // 데이터 수신 (1회 읽기)
    int nbytes = read(s, &frame, sizeof(struct can_frame));

    if (nbytes < 0) {
        perror("CAN read");
        close(s);
        return -1;
    }

    // CAN 데이터 출력
    std::cout << "CAN ID: 0x" << std::hex << frame.can_id << std::dec;
    std::cout << " DLC: " << static_cast<int>(frame.can_dlc) << " Data: ";

    for (int i = 0; i < frame.can_dlc; i++)
        printf("%02X ", frame.data[i]);
    std::cout << std::endl;

    // 데이터(int형)로 변환
    int value = 0;
    if(frame.can_dlc >= 4) {
        memcpy(&value, frame.data, sizeof(int));
        std::cout << "수신한 int 값: " << value << std::endl;
    } else {
        std::cerr << "CAN 데이터 길이(DLC)가 4바이트 미만입니다." << std::endl;
        close(s);
        return -1;
    }

    close(s);
    return value; // 정수값 반환
}

#include "multi-event-example.hpp"
#include <unistd.h>

#ifndef VSOMEIP_ENABLE_SIGNAL_HANDLING
service_sample *its_sample_ptr = nullptr;
service_sample *speed_ptr = nullptr;
service_sample *battery_ptr = nullptr;

void handle_signal(int _signal) {
    if (its_sample_ptr != nullptr &&
        (_signal == SIGINT || _signal == SIGTERM)) {
        its_sample_ptr->stop();
    }
    if (speed_ptr != nullptr && (_signal == SIGINT || _signal == SIGTERM)) {
        speed_ptr->stop();
    }
    if (battery_ptr != nullptr && (_signal == SIGINT || _signal == SIGTERM)) {
        battery_ptr->stop();
    }

}
#endif

int processInit(int _service=0) {
    uint32_t cycle = 10;

    // service_sample its_sample(cycle, _service);

    if (_service == SPEED) {
        service_sample speed(cycle, _service, SPEED_INSTANCE_ID, SPEED_EVENT_ID);
#ifndef VSOMEIP_ENABLE_SIGNAL_HANDLING
    speed_ptr = &speed;
    signal(SIGINT, handle_signal);  // Ctrl+C 처리
    signal(SIGTERM, handle_signal); // 종료 신호 처리
#endif
    if (_service == SPEED && speed.speedInit()) {
        speed.start();
#ifdef VSOMEIP_ENABLE_SIGNAL_HANDLING
        speed.stop();  // 신호 처리 종료
#endif
    return 0;
    } else 
        return 1;
    }

    else if (_service == BATTERY) {
        service_sample battery(cycle, _service, BATTERY_INSTANCE_ID, BATTERY_EVENT_ID);

#ifndef VSOMEIP_ENABLE_SIGNAL_HANDLING
        battery_ptr = &battery;    
    signal(SIGINT, handle_signal);  // Ctrl+C 처리
    signal(SIGTERM, handle_signal); // 종료 신호 처리
#endif
    
    if (battery.batteryInit()) {
        battery.start();
#ifdef VSOMEIP_ENABLE_SIGNAL_HANDLING
        battery.stop();  // 신호 처리 종료
#endif
    return 0;
    } else
        return 1;
    }

    // std::cout << "SERVER : Service value : "<< its_sample.getService() << std::endl;
//     if (its_sample.init()) {
//         its_sample.start(); // 서비스 시작

// #ifdef VSOMEIP_ENABLE_SIGNAL_HANDLING
//         its_sample.stop();  // 신호 처리 종료
// #endif
//         return 0;
//     } else {
//         return 1; // 초기화 실패
//     }

}
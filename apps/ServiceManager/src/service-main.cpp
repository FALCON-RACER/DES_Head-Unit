#include "service-example.hpp"
#include <unistd.h>


#ifndef VSOMEIP_ENABLE_SIGNAL_HANDLING
service_sample *its_sample_ptr = nullptr;

void handle_signal(int _signal) {
    if (its_sample_ptr != nullptr &&
        (_signal == SIGINT || _signal == SIGTERM)) {
        its_sample_ptr->stop();
    }
}
#endif


/*
    // int main() {
    //     uint32_t cycle = 10; // 주기 조절. 1000 = 1초.

    //     std::string cycle_arg("--cycle");

    //     service_sample its_sample(cycle);

    // #ifndef VSOMEIP_ENABLE_SIGNAL_HANDLING
    //     its_sample_ptr = &its_sample;
    //     signal(SIGINT, handle_signal);  // Ctrl+C 처리
    //     signal(SIGTERM, handle_signal); // 종료 신호 처리
    // #endif

    //     if (its_sample.init()) {
    //         its_sample.start(); // 서비스 시작
    // #ifdef VSOMEIP_ENABLE_SIGNAL_HANDLING
    //         its_sample.stop();  // 신호 처리 종료
    // #endif
    //         return 0;
    //     } else {
    //         return 1; // 초기화 실패
    //     }
    // }
*/
// 1 : speed, 2 : battery, 0 (default) : gear
int processInit(int _service=0) {
    uint32_t cycle = 10;

    std::string cycle_arg("--cycle");

    service_sample its_sample(cycle, _service);
#ifndef VSOMEIP_ENABLE_SIGNAL_HANDLING
    its_sample_ptr = &its_sample;
    signal(SIGINT, handle_signal);  // Ctrl+C 처리
    signal(SIGTERM, handle_signal); // 종료 신호 처리
#endif

    // std::cout << "SERVER : Service value : "<< its_sample.getService() << std::endl;
    if (its_sample.init()) {
        its_sample.start(); // 서비스 시작
#ifdef VSOMEIP_ENABLE_SIGNAL_HANDLING
        its_sample.stop();  // 신호 처리 종료
#endif
        return 0;
    } else {
        return 1; // 초기화 실패
    }

}
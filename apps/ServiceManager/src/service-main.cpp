// #include "service-example.hpp"

#include "server.hpp"
#include "./speed-src/speed/speed.hpp"
#include "./battery-src/battery/battery.hpp"
#include "./gear-src/gear/gear.hpp"


#ifndef VSOMEIP_ENABLE_SIGNAL_HANDLING
speedObject *speedInit_ptr = nullptr;
batteryObject *batteryInit_ptr = nullptr;
gearObject *gearInit_ptr = nullptr;

void common_handle_signal(int _signal) {
    if (speedInit_ptr != nullptr && (_signal == SIGINT || _signal == SIGTERM)) {
        speedInit_ptr->stop();
    }
    if (batteryInit_ptr != nullptr && (_signal == SIGINT || _signal == SIGTERM)) {
        batteryInit_ptr->stop();
    }
    if (gearInit_ptr != nullptr && (_signal == SIGINT || _signal == SIGTERM)) {
        gearInit_ptr->stop();
    }    
}
#endif

int speedProcessInit() {
    uint32_t cycle = 10;
    speedObject speedInit(cycle);
    
#ifndef VSOMEIP_ENABLE_SIGNAL_HANDLING
    speedInit_ptr = &speedInit;
    signal(SIGINT, common_handle_signal);  // Ctrl+C handle 
    signal(SIGTERM, common_handle_signal); // end signal handle
#endif

    if (speedInit.init()) {
        speedInit.start();
#ifdef VSOMEIP_ENABLE_SIGNAL_HANDLING
        speedInit.stop();
#endif
        return 0;
    } else {
        return 1;
    }
}

int batteryProcessInit() {
    uint32_t cycle = 10;
    batteryObject batteryInit(cycle);

#ifndef VSOMEIP_ENABLE_SIGNAL_HANDLING
    batteryInit_ptr = &batteryInit;
    signal(SIGINT, common_handle_signal);
    signal(SIGTERM, common_handle_signal);
#endif

    if (batteryInit.init()) {
        batteryInit.start();
#ifdef VSOMEIP_ENABLE_SIGNAL_HANDLING
        batteryInit.stop();
#endif
        return 0;
    } else {
        return 1;
    }
}

int gearProcessInit() {
    uint32_t cycle = 10;
    gearObject gearInit(cycle);

#ifndef VSOMEIP_ENABLE_SIGNAL_HANDLING
    gearInit_ptr = &gearInit;
    signal(SIGINT, common_handle_signal);
    signal(SIGTERM, common_handle_signal);
#endif

    if (gearInit.init()) {
        gearInit.start();
#ifdef VSOMEIP_ENABLE_SIGNAL_HANDLING
        gearInit.stop();
#endif
        return 0;
    } else {
        return 1;
    }
}
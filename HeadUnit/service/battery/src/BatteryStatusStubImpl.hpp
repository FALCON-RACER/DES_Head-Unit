#ifndef BATTERYSTATUSSTUBIMPL_H_
#define BATTERYSTATUSSTUBIMPL_H_
#include <CommonAPI/CommonAPI.hpp>
#include </home/kiku/Head_Unit/battery/src-gen/core/skel/battery/v1/commonapi/BatteryStatusStubDefault.hpp>

class BatteryStatusStubImpl: public v1_0::commonapi::BatteryStatusStubDefault {
public:
    BatteryStatusStubImpl();
    virtual ~BatteryStatusStubImpl();
};
#endif

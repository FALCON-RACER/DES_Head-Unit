#include "SpeedStubImpl.hpp"

#include <iostream>
#include <thread>
#include <CommonAPI/CommonAPI.hpp>
#include <unistd.h>

using namespace v1::commonapi;

class Speed {
public:
    Speed();
    ~Speed();
    void setSpeed(float speed);

private:
    std::shared_ptr<CommonAPI::Runtime> runtime;
    std::shared_ptr<SpeedStubImpl> speedService;

    void speedServiceInit();
};

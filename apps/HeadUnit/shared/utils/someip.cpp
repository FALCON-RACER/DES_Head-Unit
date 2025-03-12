#include "./someip.h"

// void SomeIP::send_gear_data(int gear)
// {
//     std::thread sender(
//         [this, &gear]() 
//         {
//         std::cout << "Gear Value in Run function : " << gear << std::endl;
//         std::shared_ptr<vsomeip::message> request = vsomeip::runtime::get()->create_request();
//         if (!request) {
//             std::cerr << "Failed to create request." << std::endl;
//             return;
//         }
//         request->set_service(VEHICLE_SERVICE_ID);
//         request->set_instance(GEAR_INSTANCE_ID);
//         request->set_method(GEAR_SET_METHOD_ID);

//         std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
//         if (!its_payload) {
//             std::cerr << "Failed to create payload." << std::endl;
//             return;
//         }
//         std::vector<vsomeip::byte_t> its_payload_data(
//             reinterpret_cast<vsomeip::byte_t*>(&gear),
//             reinterpret_cast<vsomeip::byte_t*>(&gear) + sizeof(int));
//         its_payload->set_data(its_payload_data);
//         request->set_payload(its_payload);
//         app->send(request);
//         std::cout << "HEAD UNIT : GEAR DATA SENDED" << std::endl;
//         std::this_thread::sleep_for(std::chrono::seconds(1));
//     }
// );
//     app->start();
//     // sender.join();  // 스레드 종료 대기
// }

void SomeIP::set_gear_data(int gear) {
    std::cout << "[set gear data function] value : "<< gear << std::endl;
    // this->preValue = this->value;
    // this->value = gearValue;

    std::shared_ptr<vsomeip::message> request;
    
    request = vsomeip::runtime::get()->create_request();
    request->set_service(VEHICLE_SERVICE_ID);
    request->set_instance(GEAR_INSTANCE_ID);
    request->set_method(GEAR_SET_METHOD_ID);

    // std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
    // if (!its_payload) {
    //     std::cerr << "Failed to create payload." << std::endl;
    //     return;
    // }
    std::shared_ptr< vsomeip::payload > its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data(
      reinterpret_cast<vsomeip::byte_t *>(&gear),
      reinterpret_cast<vsomeip::byte_t *>(&gear) + sizeof(int));

        
    its_payload->set_data(its_payload_data);
    request->set_payload(its_payload);
    app->send(request);
    std::cout << "HEAD UNIT : GEAR DATA SENDED" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return ;
}

void SomeIP::run() {
    std::cout << "Gear Value in Run function" << std::endl;
    int gear;
    std::shared_ptr<vsomeip::message> request = vsomeip::runtime::get()->create_request();
    if (!request) {
        std::cerr << "Failed to create request." << std::endl;
        return;
    }
    request->set_service(VEHICLE_SERVICE_ID);
    request->set_instance(GEAR_INSTANCE_ID);
    request->set_method(GEAR_SET_METHOD_ID);

    std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
    if (!its_payload) {
        std::cerr << "Failed to create payload." << std::endl;
        return;
    }
    std::cout << "waiting for the value..." << std::endl;
    if (this->value != this->preValue) {
        gear = this->value;
        this->preValue = gear;
        std::vector<vsomeip::byte_t> its_payload_data(
            reinterpret_cast<vsomeip::byte_t*>(&gear),
            reinterpret_cast<vsomeip::byte_t*>(&gear) + sizeof(int));
        its_payload->set_data(its_payload_data);
        request->set_payload(its_payload);
        app->send(request);
        std::cout << "HEAD UNIT : GEAR DATA SENDED" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available) {
    std::cout << (_is_available ? "gear available." : "gear NOT available.") << std::endl;
}

void SomeIP::start() {
    std::thread someIP_thread([this](){
        // std::thread sender(std::bind(&SomeIP::run, this));
        ;
        app->start();});
    someIP_thread.detach();
}

bool SomeIP::init() {
    if (!app->init())
    {
        std::cerr << "Couldn't initialize application" << std::endl;
        return false;
    }   
    app->register_availability_handler(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID, on_availability);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    app->request_service(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID);
    this->start();
    return true;
}
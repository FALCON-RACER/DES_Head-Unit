#include "./someip.h"

void SomeIP::set_gear_data(int gear) {
    std::cout << "[set gear data function] value : "<< gear << std::endl;
    std::shared_ptr<vsomeip::message> request;
    
    request = vsomeip::runtime::get()->create_request();
    request->set_service(VEHICLE_SERVICE_ID);
    request->set_instance(GEAR_INSTANCE_ID);
    request->set_method(GEAR_SET_METHOD_ID);

    std::shared_ptr< vsomeip::payload > its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data(
      reinterpret_cast<vsomeip::byte_t *>(&gear),
      reinterpret_cast<vsomeip::byte_t *>(&gear) + sizeof(int));

        
    its_payload->set_data(its_payload_data);
    request->set_payload(its_payload);
    app->send(request);
    std::cout << "HEAD UNIT : GEAR DATA SENDED" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(90));
    return ;
}

void on_gear_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available) {
    std::cout << (_is_available ? "gear available." : "gear NOT available.") << std::endl;
}

void SomeIP::start() {
    std::thread someIP_thread([this](){
        app->start();});
    someIP_thread.detach();
}

bool SomeIP::init() {
    if (!app->init())
    {
        std::cerr << "Couldn't initialize application" << std::endl;
        return false;
    }   
    app->register_availability_handler(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID, on_gear_availability);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    app->request_service(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID);
    this->start();
    return true;
}
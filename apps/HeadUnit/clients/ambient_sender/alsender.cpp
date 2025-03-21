#include "./alsender.hpp"

void Alsender::set_al_data(int al) {
    std::cout << "[set al data function] value : "<< al << std::endl;
    std::shared_ptr<vsomeip::message> request;
    
    request = vsomeip::runtime::get()->create_request();
    request->set_service(VEHICLE_SERVICE_ID);
    request->set_instance(AL_INSTANCE_ID);
    request->set_method(AL_SET_METHOD_ID);

    std::shared_ptr< vsomeip::payload > its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data(
      reinterpret_cast<vsomeip::byte_t *>(&al),
      reinterpret_cast<vsomeip::byte_t *>(&al) + sizeof(int));

    its_payload->set_data(its_payload_data);
    request->set_payload(its_payload);
    app->send(request);
    std::cout << "HEAD UNIT : AL DATA SENDED" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return ;
}

void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available) {
    std::cout << (_is_available ? "AL available." : "AL NOT available.") << std::endl;
}

void Alsender::start() {
    std::thread al_thread([this](){
        app->start();});
    al_thread.detach();
}

bool Alsender::init() {
    if (!app->init())
    {
        std::cerr << "Couldn't initialize application" << std::endl;
        return false;
    }   
    app->register_availability_handler(VEHICLE_SERVICE_ID, AL_INSTANCE_ID, on_availability);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    app->request_service(VEHICLE_SERVICE_ID, AL_INSTANCE_ID);
    this->start();
    return true;
}
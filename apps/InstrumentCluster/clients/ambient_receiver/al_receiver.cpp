#include "../headers.hpp"
#include "../server.hpp"
#include "./al_receiver.hpp"
//받은 int값을 처리.
// std::shared_ptr<vsomeip::application> app;

void AlClient::on_message(const std::shared_ptr<vsomeip::message> &_request) {
  std::shared_ptr<vsomeip::payload> payload = _request->get_payload();
  int received_value = 0;

  if (payload->get_length() >= sizeof(int)) {
      received_value = *reinterpret_cast<const int*>(payload->get_data());
      std::cout << "SERVER: Received int: " << received_value << std::endl;
  } else {
      std::cerr << "SERVER: Invalid payload size!" << std::endl;
      return;
  }
  this->alValue = received_value;
}

void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available) {
  std::cout << (_is_available ? "AL receiver available." : "AL receiver NOT available.") << std::endl;
}

void AlClient::start() {
  std::thread al_thread([this](){
      app->start();});
  al_thread.detach();
}

bool AlClient::init() {
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

// int main() {
//    app = vsomeip::runtime::get()->create_application("ambient");
//    app->init();
//    std::this_thread::sleep_for(std::chrono::seconds(2));
//    app->register_message_handler(VEHICLE_SERVICE_ID, AL_INSTANCE_ID, AL_SET_METHOD_ID, on_message);
//    app->offer_service(VEHICLE_SERVICE_ID, AL_INSTANCE_ID);
//    app->start();
// }
// void AlClient::start() {
//   app = vsomeip::runtime::get()->create_application("ambient");
//   app->init();
//   std::this_thread::sleep_for(std::chrono::seconds(2));
//   app->register_message_handler(VEHICLE_SERVICE_ID, AL_INSTANCE_ID, AL_SET_METHOD_ID, on_message);
//   app->offer_service(VEHICLE_SERVICE_ID, AL_INSTANCE_ID);
//   app->start();
// }
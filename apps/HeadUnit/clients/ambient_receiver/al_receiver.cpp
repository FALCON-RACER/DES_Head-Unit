#include "../../headers.hpp"
#include "../../server.hpp"
#include "./al_receiver.hpp"
//받은 int값을 처리.
std::shared_ptr<vsomeip::application> app;

void alClient::on_message(const std::shared_ptr<vsomeip::message> &_request) {
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

// int main() {
//    app = vsomeip::runtime::get()->create_application("ambient");
//    app->init();
//    std::this_thread::sleep_for(std::chrono::seconds(2));
//    app->register_message_handler(VEHICLE_SERVICE_ID, AL_INSTANCE_ID, AL_SET_METHOD_ID, on_message);
//    app->offer_service(VEHICLE_SERVICE_ID, AL_INSTANCE_ID);
//    app->start();
// }
void alClient::start() {
  app = vsomeip::runtime::get()->create_application("ambient");
  app->init();
  std::this_thread::sleep_for(std::chrono::seconds(2));
  app->register_message_handler(VEHICLE_SERVICE_ID, AL_INSTANCE_ID, AL_SET_METHOD_ID, on_message);
  app->offer_service(VEHICLE_SERVICE_ID, AL_INSTANCE_ID);
  app->start();
}
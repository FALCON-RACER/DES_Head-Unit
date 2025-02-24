#include <iomanip>
#include <iostream>
#include <sstream>

#include <condition_variable>
#include <thread>

#include <vsomeip/vsomeip.hpp>
#include "../server.hpp"

std::shared_ptr< vsomeip::application > app;
std::mutex mutex;
std::condition_variable condition;

//sending the actual data (will be gear data) to server.
void run() {
  std::unique_lock<std::mutex> its_lock(mutex);
  condition.wait(its_lock);

  std::shared_ptr< vsomeip::message > request;
  request = vsomeip::runtime::get()->create_request();
  request->set_service(VEHICLE_SERVICE_ID);
  request->set_instance(GEAR_INSTANCE_ID);
  request->set_method(GEAR_SET_METHOD_ID);

//sending actual data. changed into HU input value.
  int value=0;
  
  while (1){
    // value += 1;
    if (value != 5)
      {std::cout << "Input number" << std::endl;
    std::cin >> value;}
    std::shared_ptr< vsomeip::payload > its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data(
      reinterpret_cast<vsomeip::byte_t *>(&value),
      reinterpret_cast<vsomeip::byte_t *>(&value) + sizeof(int)
  );
  its_payload->set_data(its_payload_data);
  request->set_payload(its_payload);
  app->send(request);
  std::this_thread::sleep_for(std::chrono::seconds(1));

  }
  std::cout << "CLIENT : DATA SENDED" << std::endl;
}

// When response come, print the payload from server.
void on_message(const std::shared_ptr<vsomeip::message> &_response) {
  std::shared_ptr<vsomeip::payload> its_payload = _response->get_payload();
  vsomeip::length_t l = its_payload->get_length();

  // Get payload
  std::stringstream ss;
  for (vsomeip::length_t i=0; i<l; i++) {
     ss << std::setw(2) << std::setfill('0') << std::hex
        << (int)*(its_payload->get_data()+i) << " ";
  }

  std::cout << "CLIENT: Received message with Client/Session ["
      << std::setw(4) << std::setfill('0') << std::hex << _response->get_client() << "/"
      << std::setw(4) << std::setfill('0') << std::hex << _response->get_session() << "] "
      << ss.str() << std::endl;
}

void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available) {
    std::cout << "CLIENT: Service ["
            << std::setw(4) << std::setfill('0') << std::hex << _service << "." << _instance
            << "] is "
            << (_is_available ? "available." : "NOT available.")
            << std::endl;
    if (_is_available) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        condition.notify_one();
    }
}

int main() {

    app = vsomeip::runtime::get()->create_application("gear");
    app->init();
    app->register_availability_handler(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID, on_availability);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    app->request_service(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID);
    app->register_message_handler(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID, JOY_GEAR_RESPONSE_MID, on_message);
    std::thread sender(run);
    app->start();
}

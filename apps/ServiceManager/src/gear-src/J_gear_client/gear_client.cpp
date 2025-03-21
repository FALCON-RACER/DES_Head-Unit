#include <iomanip>
#include <iostream>
#include <sstream>

#include <condition_variable>
#include <thread>
#include <vsomeip/vsomeip.hpp>

#include "../../server.hpp"

std::shared_ptr< vsomeip::application > app;
std::mutex mutex;
std::condition_variable condition;

//TODO: sending the actual data (will be gear data) to server.

// VEHICLE_SERVICE_ID, etc IDs are defined in src/server.hpp

// void run(int gearValue) {
void run() {
  // on avaliability notify this thread that client is connected to server
  std::unique_lock<std::mutex> its_lock(mutex);
  condition.wait(its_lock);

  //create request
  std::shared_ptr< vsomeip::message > request;
  request = vsomeip::runtime::get()->create_request();
  //basic setting
  request->set_service(VEHICLE_SERVICE_ID);
  request->set_instance(GEAR_INSTANCE_ID);
  request->set_method(JOY_GEAR_SET_MID);

//sending actual data. changed into HU input value.
  int value=0; // <- real gear value should be on value variable
  while (1){
    std::cout << "Input number" << std::endl;
    std::cin >> value;

    std::shared_ptr< vsomeip::payload > its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data(
      reinterpret_cast<vsomeip::byte_t *>(&value),
      reinterpret_cast<vsomeip::byte_t *>(&value) + sizeof(int)
  );
  its_payload->set_data(its_payload_data);
  request->set_payload(its_payload);
  app->send(request);
  std::cout << "CLIENT : DATA SENDED" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(1));

  }
}
// PARKING 0
// REVERSE 1
// NEUTRAL 2
// DRIVE 3

// When response come, print the payload from server.
void on_message(const std::shared_ptr<vsomeip::message> &_response) {
    std::shared_ptr<vsomeip::payload> payload = _response->get_payload();
    int received_value = 0;

    if (payload->get_length() >= sizeof(int)) {
        received_value = *reinterpret_cast<const int*>(payload->get_data());
        std::cout << "SERVER: Received int: " << received_value << std::endl;
    } else {
        std::cerr << "SERVER: Invalid payload size!" << std::endl;
        return;
    }
    //
}

// 서버랑 연결이 안되어있을때 전송을 할수도있어 
// vsomeip는 /tmp/vsomeip-100 103~~~ 
// 얘가 소켓이 꼬여. 그래서 데이터 안가 제대로 연결이 됐어도 그래서 그냥 두는게 나을거같다.
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
    std::this_thread::sleep_for(std::chrono::seconds(1));
    app->register_message_handler(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID, JOY_GEAR_SET_MID, on_message);
    std::thread sender(run);
    app->start();
}

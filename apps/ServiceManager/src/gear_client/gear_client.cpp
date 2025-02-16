#include <iomanip>
#include <iostream>
#include <sstream>

#include <condition_variable>
#include <thread>

#include <vsomeip/vsomeip.hpp>
#include "../sample-ids.hpp"


std::shared_ptr< vsomeip::application > app;
std::mutex mutex;
std::condition_variable condition;

void on_message(const std::shared_ptr<vsomeip::message> response) {
  //전송한 데이터에 대한 응답. y or n로 받아서 조건부처리.
    // auto payload = response->get_payload();
    // std::vector<uint8_t> response_data(payload->get_data(), payload->get_data() + payload->get_length());

    // std::cout << "Received response: ";
    // for (auto byte : response_data) std::cout << std::hex << (int)byte << " ";
    // std::cout << std::endl;
}

void send_request() {
  //data set part.
    // auto request = vsomeip::runtime::get()->create_request();
    // auto payload = vsomeip::runtime::get()->create_payload();
    
    // std::vector<uint8_t> request_data = {0x10, 0x20, 0x30, 0x40}; // 변경된 데이터
    // payload->set_data(request_data);
    
    request->set_payload(payload);
    request->set_service(VEHICLE_SERVICE_ID);
    request->set_instance(GEAR_INSTANCE_ID);
    request->set_method(SET_METHOD_ID);

    std::cout << "Sending request with updated data..." << std::endl;
    app->send(request);
}



int main() {
    app = vsomeip::runtime::get()->create_application("Gear");

    app->init();
    // Using Set method.
    app->register_message_handler(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID, SET_METHOD_ID, on_message);

//데이터를 보내기 위해 상대방을 인식하는 절차
    app->request_service(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID);
    std::this_thread::sleep_for(std::chrono::seconds(2)); // 서비스 검색 시간

    // 데이터 변경 감지 시 요청 전송 (예제에서는 3초 후 전송)
    std::this_thread::sleep_for(std::chrono::seconds(3));
    send_request();

    app->start();   
}
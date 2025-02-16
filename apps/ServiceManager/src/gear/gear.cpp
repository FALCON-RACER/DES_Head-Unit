#include "./gear.hpp"
#include <vsomeip/vsomeip.hpp>
#include <iostream>
#include <memory>


// 기어 처리 파트는 request, response 구조로 되어있기에,
// 클라이언트가 서버에 request를 보내는 구조로 되어있다.
std::shared_ptr<vsomeip::application> app;

void on_request(const std::shared_ptr<vsomeip::message> request) {
    auto payload = request->get_payload();
    std::vector<uint8_t> request_data(payload->get_data(), payload->get_data() + payload->get_length());

    std::cout << "Received request data: ";
    for (auto byte : request_data) std::cout << std::hex << (int)byte << " ";
    std::cout << std::endl;

    // 응답 생성
    auto response = vsomeip::runtime::get()->create_response(request);
    auto response_payload = vsomeip::runtime::get()->create_payload();
    std::vector<uint8_t> response_data = {0xAA, 0xBB, 0xCC}; // 응답 데이터
    response_payload->set_data(response_data);
    response->set_payload(response_payload);

    // 클라이언트로 응답 전송
    app->send(response);
}

int main() {
    app = vsomeip::runtime::get()->create_application("GearServer");
    app->init();
    
    app->register_message_handler(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID, SET_METHOD_ID, on_request);
    app->offer_service(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID);
    
    app->start();
    return 0;
}

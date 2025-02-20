#include "../../headers.hpp"
#include "../../server.hpp"

std::shared_ptr<vsomeip::application> app;

// Data come from HU.
void on_message(const std::shared_ptr<vsomeip::message> &_request) {

    std::shared_ptr<vsomeip::payload> its_payload = _request->get_payload();
    vsomeip::length_t l = its_payload->get_length();

    // Get payload
    std::stringstream ss;
    for (vsomeip::length_t i=0; i<l; i++) {
       ss << std::setw(2) << std::setfill('0') << std::hex
          << (int)*(its_payload->get_data()+i) << " ";
    }

    // std::cout << "SERVICE: Received message with Client/Session ["
    //     << std::setw(4) << std::setfill('0') << std::hex << _request->get_client() << "/"
    //     << std::setw(4) << std::setfill('0') << std::hex << _request->get_session() << "] "
    //     << ss.str() << std::endl;

    // Create response. send it to IC.
    std::shared_ptr<vsomeip::message> its_response = vsomeip::runtime::get()->create_response(_request);
    its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    for (int i=9; i>=0; i--) {
        its_payload_data.push_back(i % 256);
    }
    its_payload->set_data(its_payload_data);
    its_response->set_payload(its_payload);
    app->send(its_response);

  std::shared_ptr< vsomeip::message > request;
  request = vsomeip::runtime::get()->create_request();
  request->set_service(VEHICLE_SERVICE_ID);
  request->set_instance(AL_INSTANCE_ID);
  request->set_method(AL_SET_METHOD_ID);

  std::shared_ptr< vsomeip::payload > its_payload = vsomeip::runtime::get()->create_payload();
  std::vector< vsomeip::byte_t > its_payload_data;
  for (vsomeip::byte_t i=0; i<10; i++) {
      its_payload_data.push_back(i % 256);
  }
   
  its_payload->set_data(its_payload_data);
  request->set_payload(its_payload);
  app->send(request);


}

int main() {

   app = vsomeip::runtime::get()->create_application("World");
   app->init();
   std::this_thread::sleep_for(std::chrono::seconds(2));
   app->register_message_handler(VEHICLE_SERVICE_ID, AL_INSTANCE_ID, AL_SET_METHOD_ID, on_message);
   app->offer_service(VEHICLE_SERVICE_ID, AL_INSTANCE_ID);
   app->start();
}
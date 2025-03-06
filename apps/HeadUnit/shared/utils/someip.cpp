#include "./someip.h"

std::shared_ptr< vsomeip::application > app;
std::mutex mutex;
std::condition_variable condition;

void SomeIP::send_gear_data(int gear)
{
	std::unique_lock<std::mutex> its_lock(mutex);
	condition.wait(its_lock);

	std::shared_ptr<vsomeip::message> request;
	request = vsomeip::runtime::get()->create_request();
	request->set_service(VEHICLE_SERVICE_ID);
	request->set_instance(GEAR_INSTANCE_ID);
	request->set_method(GEAR_SET_METHOD_ID);

	std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
	std::vector<vsomeip::byte_t> its_payload_data(
		reinterpret_cast<vsomeip::byte_t *>(&gear),
		reinterpret_cast<vsomeip::byte_t *>(&gear) + sizeof(int));
	its_payload->set_data(its_payload_data);
	request->set_payload(its_payload);
	app->send(request);
	std::cout << "HEAD UNIT : GEAR DATA SENDED" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

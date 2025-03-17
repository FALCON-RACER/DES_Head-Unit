#ifndef SPEED_HPP
# define SPEED_HPP

#include "../../service_base/service-base.hpp"
#include "./canreceiver.hpp"

class speedObject : public service_sample
{  
public :
  virtual bool init();
  virtual void start();
  virtual void stop();
  virtual void offer();
  virtual void stop_offer();
  virtual void run();
  virtual void notify();
  virtual void on_state(vsomeip::state_type_e _state);
  virtual void on_get(const std::shared_ptr<vsomeip::message> &_message);
  virtual void on_set(const std::shared_ptr<vsomeip::message> &_message);

  speedObject(uint32_t _cycle);

private :
  float speedData;

  void canDataReceive();

  std::shared_ptr<vsomeip::application> app_;
  std::shared_ptr<vsomeip::payload> payload_;
  
  bool is_registered_;
  uint32_t cycle_;

  bool blocked_;
  bool running_;
  bool is_offered_;

  std::condition_variable condition_;
  std::condition_variable notify_condition_;
  std::condition_variable CAN_condition_; 
  std::condition_variable battery_condition_;

  std::mutex mutex_;
  std::mutex notify_mutex_;
  std::mutex payload_mutex_;
  std::mutex can_mutex_;
  std::mutex battery_mutex_;

  std::thread offer_thread_;
  std::thread notify_thread_;
  std::thread CAN_thread_;
  std::thread battery_thread_;

};

void handle_signal(int _signal);

#endif
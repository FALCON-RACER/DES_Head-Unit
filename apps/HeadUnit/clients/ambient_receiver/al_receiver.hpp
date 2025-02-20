#ifndef AL_RECEIVER_HPP
# define AL_RECEIVER_HPP

class alClient
{
private :
  int alValue;
  void on_message(const std::shared_ptr<vsomeip::message> &_request);
  void start();
  
public :
  alClient(void);
  ~alClient(void);
};

#endif
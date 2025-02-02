// #include "client-example.hpp"
#include "multi-event-example.hpp"
#include "../sample-ids.hpp"

#include <unistd.h>
#include <iostream>

int main(int argc, char **argv)
{
  pid_t speed_pid; // 1
  pid_t battery_pid; // 2
  pid_t gear_pid; // 3
  // pid_t ambientLight_pid; // 4
  
  // processInit(1);
  // processInit(SPEED);
  // processInit(BATTERY);

  speed_pid = fork();
  if (speed_pid == 0)
    processInit(1);
    //speed를 받아오는 프로세스
  
  if (speed_pid != 0)
    battery_pid = fork();

  //battery 처리 프로세스
  if (battery_pid == 0)
    processInit(2);  
  
  //시그널 처리 시 프로세스 회수 추가.
  
  // if (speed_pid != 0 && battery_pid != 0) {
  //   gear_pid = fork();

  //   //gear 처리 프로세스 실행.
  //   if (gear_pid == 0)
  //     processInit(3);
  // }
  
  return true;

}
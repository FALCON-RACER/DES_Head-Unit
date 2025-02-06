#include "sample-ids.hpp"
#include "./speed/speed.hpp"

#include <unistd.h>
#include <iostream>
#include <csignal>
#include <vector>
#include <sys/wait.h>


pid_t speed_pid; // 1
pid_t battery_pid; // 2
pid_t gear_pid; // 3

//TODO: kill child process when sigint detected. + vsomeip routing manager edit

int main(int argc, char **argv)
{
  // signal(SIGINT, handle_signal); // SIGINT 시그널 핸들러 등록
  // pid_t ambientLight_pid; // 4

  speed_pid = fork();
  if (speed_pid == 0)
    speedProcessInit();
  
  if (speed_pid != 0)
    battery_pid = fork();
  // //battery 처리 프로세스
  if (battery_pid == 0)
    batteryProcessInit();  

  // if (speed_pid != 0 && battery_pid != 0) {
  //   gear_pid = fork();

  //   //gear 처리 프로세스 실행.
  //   if (gear_pid == 0)
  //     processInit(3);
  // }
  
  return true;

}
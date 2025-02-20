#include "./headers.hpp"
#include "./server.hpp"
#include "./speed-src/speed/speed.hpp"
#include "./battery-src/battery/battery.hpp"
#include "./gear-src/gear/gear.hpp"


pid_t speed_pid, battery_pid, gear_pid;

void handle_signal(int signal) {
    if (speed_pid > 0) {
        std::cout << "SPEED PROCESS KILLED" << std::endl;
        kill(speed_pid, SIGTERM);
    }
    if (battery_pid > 0) {
        std::cout << "BATTERY PROCESS KILLED" << std::endl;
        kill(battery_pid, SIGTERM);
    }
    if (gear_pid > 0){
        std::cout << "GEAR PROCESS KILLED" << std::endl;
        kill(gear_pid, SIGTERM);
    }
    while (wait(NULL) > 0);

    exit(0);
}

//speed, battery, gear init is defined on service-base.hpp.

int main(int argc, char **argv) {
    signal(SIGINT, handle_signal);  // SIGINT (Ctrl+C) 처리
    signal(SIGTERM, handle_signal); // SIGTERM (kill 명령어 등) 처리

    speed_pid = fork();
    if (speed_pid == 0) {
        std::cout << "SERVER : speed started" << std::endl;
        speedProcessInit();
        exit(0); // speedProcessInit()이 종료되면, 자식 프로세스 종료
    }

    if (speed_pid != 0)
        battery_pid = fork();

    if (battery_pid == 0) {
        std::cout << "SERVER : battery started" << std::endl;
        batteryProcessInit();
        exit(0); // batteryProcessInit()이 종료되면, 자식 프로세스 종료
    }

  if (speed_pid != 0 && battery_pid != 0) {
    gear_pid = fork();
    //gear 처리 프로세스 실행.
    if (gear_pid == 0) {
        std::cout << "SERVER : gear started" << std::endl;
        gearProcessInit();
        exit(0);
        }
    }
    // 부모 프로세스는 자식 프로세스가 종료될 때까지 대기
    while (wait(NULL) > 0);
    std::cout << "Parent process exiting." << std::endl;
    return 0;
}

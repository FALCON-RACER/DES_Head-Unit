#include "../header/main.hpp"

#include <iostream>
#include <vsomeip/vsomeip.hpp>
#include <iomanip>

#include "../header/IC/speedupdatemanager.h"
#include "../header/IC/batterychecker.hpp"

int main()
{
  BatteryChecker batteryChecker;
  SpeedUpdateManager speedUpdateManager;

  batteryChecker.startMonitoring();
  SpeedUpdateManager.start();
  
}
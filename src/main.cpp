#include "WatchdogMonitor.h"
#include <iostream>
#include <thread>

void Thread1(const std::unique_ptr<watchdog::IWatchdogMonitor> &watchdog,
             const watchdog::Identity_t &aIdentity) {
  std::thread t1([&watchdog, &aIdentity]() {
    while (1) {
      watchdog->kick(aIdentity);
      std::this_thread::sleep_for(std::chrono::seconds(30));
    }
  });

  t1.join();
}

int main() {

  auto missingCb = [](watchdog::Identity_t aIdentity) {
    std::cout << aIdentity << " failed to kick watchdog.. restart \n";
  };
  watchdog::IWatchdogMonitor::Config cfg{"TestingProcess", 5, 2, missingCb};
  auto wdog = watchdog::CreateWatchdog(cfg);
  const auto identity = wdog->RegisterEntity("Thread1");

  Thread1(std::ref(wdog), std::ref(identity));

  return 0;
}
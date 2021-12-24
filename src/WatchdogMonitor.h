#pragma once

#include "../interface/IwatchdogMonitor.h"
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <unordered_set>

namespace watchdog {
class CWatchdogMonitor : public IWatchdogMonitor {
public:
  explicit CWatchdogMonitor(IWatchdogMonitor::Config aConfig) noexcept;
  [[nodiscard]] Identity_t
  RegisterEntity(std::string aIdentifier) noexcept override;
  [[nodiscard]] bool UnregisterEntity(Identity_t aIdentity) noexcept override;
  void kick(Identity_t aIdentity) noexcept override;
  ~CWatchdogMonitor();

private:
  void StartMonitoring() noexcept;
  void CheckHealthAndUpdateState() noexcept;

  const IWatchdogMonitor::Config mConfig;
  std::unordered_set<Identity_t> mRegisteredEntity;
  std::unordered_map<Identity_t, std::pair<bool, uint32_t>> mEntitiesKickStatus;
  mutable std::mutex mWatchdogMtx;
  std::thread mMonitorThread;
  std::atomic<bool> mKeepMonitoring{true};
  std::condition_variable mMonitorCv;
};

} // namespace watchdog

#include "WatchdogMonitor.h"
#include <cassert>
#include <functional>
#include <iostream>

#define LOG_INFO(X)                                                            \
  std::cout << __PRETTY_FUNCTION__ << "::" << __LINE__ << " " << X << std::endl;
#define LOG_ERROR(X)                                                           \
  std::cerr << __PRETTY_FUNCTION__ << "::" << __LINE__ << " " << X << std::endl;

namespace {
std::size_t GenerateIdentity(const std::string &aIdentifier) {
  std::hash<std::string> hasher;
  return hasher(aIdentifier);
}
} // namespace

namespace watchdog {
using namespace std::chrono_literals;

std::unique_ptr<IWatchdogMonitor>
CreateWatchdog(IWatchdogMonitor::Config aConfig) noexcept {

  if (aConfig.checkupTimeInSec < 0) {
    return std::unique_ptr<CWatchdogMonitor>();
  }

  return std::make_unique<CWatchdogMonitor>(std::move(aConfig));
}

CWatchdogMonitor::CWatchdogMonitor(IWatchdogMonitor::Config aConfig) noexcept
    : mConfig(std::move(aConfig)) {

  mMonitorThread =
      std::thread(std::bind(&CWatchdogMonitor::StartMonitoring, this));
}

CWatchdogMonitor::~CWatchdogMonitor() {
  std::unique_lock<std::mutex> lock(mWatchdogMtx);
  mKeepMonitoring = false;
  mMonitorCv.notify_one();
  mMonitorThread.join();
}

void CWatchdogMonitor::StartMonitoring() noexcept {

  LOG_INFO("Monitor thread started");

  while (mKeepMonitoring) {

    std::unique_lock<std::mutex> lock(mWatchdogMtx);

    const auto status =
        mMonitorCv.wait_for(lock, mConfig.checkupTimeInSec * 1s);

    if (status == std::cv_status::timeout) {
      CheckHealthAndUpdateState();

    } else if (!mKeepMonitoring.load()) {
      LOG_INFO("Received signal to stop monitoring, exiting");
      break;
    }
  }
}

void CWatchdogMonitor::CheckHealthAndUpdateState() noexcept {

  for (auto it = mEntitiesKickStatus.begin(); it != mEntitiesKickStatus.end();
       ++it) {
    if (it->second.second == mConfig.missCountThreshold) {
      // Ideally the monitor, should restart.
      LOG_ERROR("Entity " << it->first << " has reached the missing threshold");
      mConfig.missedKickCb(it->first);
      it->second = {false, 0};
    } else {
      it->second.second += 1;
    }
  }
}

[[nodiscard]] Identity_t
CWatchdogMonitor::RegisterEntity(std::string aIdentifier) noexcept {
  Identity_t identity = GenerateIdentity(aIdentifier);

  std::unique_lock<std::mutex> lock(mWatchdogMtx);
  const auto status = mRegisteredEntity.insert(GenerateIdentity(aIdentifier));

  if (!status.second) {
    LOG_ERROR("Trying to register duplicate entity");
    identity = std::numeric_limits<Identity_t>::max();
  }

  return identity;
}

[[nodiscard]] bool
CWatchdogMonitor::UnregisterEntity(Identity_t aIdentity) noexcept {

  bool status = false;

  std::unique_lock<std::mutex> lock(mWatchdogMtx);
  if (mRegisteredEntity.erase(aIdentity) > 0) {
    status = true;
    const auto isRemoved = mEntitiesKickStatus.erase(aIdentity) > 0;
    assert(isRemoved);
  } else {
    LOG_ERROR("Trying to remove unregistered entity");
  }

  return status;
}

void CWatchdogMonitor::kick(Identity_t aIdentity) noexcept {

  std::unique_lock<std::mutex> lock(mWatchdogMtx);
  const auto isPresent = mRegisteredEntity.count(aIdentity) > 0;
  assert(isPresent);
  mEntitiesKickStatus[aIdentity] = {true, 0};
}

} // namespace watchdog

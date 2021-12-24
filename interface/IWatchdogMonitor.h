#pragma once

#include <functional>
#include <memory>
#include <string>

namespace watchdog {
using Identity_t = size_t;
using MissedKickIdentityCB = std::function<void(Identity_t)>;

class IWatchdogMonitor {
public:
  virtual ~IWatchdogMonitor() = default;
  [[nodiscard]] virtual Identity_t
  RegisterEntity(std::string aIdentifier) noexcept = 0;
  [[nodiscard]] virtual bool
  UnregisterEntity(Identity_t aIdentity) noexcept = 0;
  virtual void kick(Identity_t aIdentity) noexcept = 0;

  struct Config {
    const std::string identifier;
    const std::uint32_t checkupTimeInSec{0};
    const std::uint32_t missCountThreshold{0};
    MissedKickIdentityCB missedKickCb;
  };
};

std::unique_ptr<IWatchdogMonitor>
CreateWatchdog(IWatchdogMonitor::Config aConfig) noexcept;

} // namespace watchdog
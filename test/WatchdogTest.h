#include "gtest/gtest.h"

#include "WatchdogMonitor.h"
#include <memory>

namespace watchdog
{
class WatchdogTest: public ::testing::Test
{
    protected:
        WatchdogTest();
        ~WatchdogTest();

        void MonitorCallback(Identity_t aIdentity);
        std::unique_ptr<IWatchdogMonitor> mWatchdog;
};
}
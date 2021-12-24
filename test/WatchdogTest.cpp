#include "WatchdogTest.h"

namespace watchdog
{
    std::size_t CalculateIdentity(const std::string &aIdentity)
    {
        std::hash<std::string> hasher;
        return hasher(aIdentity);
    }

    WatchdogTest::WatchdogTest()
    {
        mWatchdog = CreateWatchdog({"WatchdogTest", 5, 2, std::bind(&WatchdogTest::MonitorCallback, this, std::placeholders::_1)});
    }

    WatchdogTest::~WatchdogTest()
    {
        mWatchdog.reset();
    }

    void WatchdogTest::MonitorCallback (Identity_t aIdentity)
    {
        std::cout << aIdentity <<  " has failed to kick in the configured\n";
    }


    TEST_F(WatchdogTest, CheckUniqueRegistration)
    {
        const auto identity = mWatchdog->RegisterEntity("FirstRegistration");
        ASSERT_EQ(identity, CalculateIdentity("FirstRegistration"));
    }

    TEST_F(WatchdogTest, CheckDuplicateRegistration)
    {
        [[maybe_unused]]const auto identity1 = mWatchdog->RegisterEntity("FirstRegistration");
        const auto identity2 = mWatchdog->RegisterEntity("FirstRegistration");

        ASSERT_EQ(identity2, std::numeric_limits<Identity_t>::max());
        (void)mWatchdog->UnregisterEntity(identity1);
    }

    TEST_F(WatchdogTest, CheckUnregistration)
    {
        const auto identity = mWatchdog->RegisterEntity("FirstRegistration");
        const auto status = mWatchdog->UnregisterEntity(identity);
        ASSERT_TRUE(status);
    }

}
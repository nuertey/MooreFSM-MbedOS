#include "mbed.h"
#include "sml.hpp" // [Boost::ext].SML

#define BLINKING_RATE 500ms
#define SLEEPING_RATE 10s

using namespace boost::sml;
namespace sml = boost::sml;

InterruptIn g_ButtonOne(SW2);
DigitalOut  g_UserLED(LED1);

bool stopBlinking{true};

// Just debug logging utilities:
struct DebugLogger_t
{
    template <class SM, class TEvent>
    void log_process_event(const TEvent&)
    {
        printf("[%s][process_event] %s\n", sml::aux::get_type_name<SM>(), sml::aux::get_type_name<TEvent>());
    }

    template <class SM, class TGuard, class TEvent>
    void log_guard(const TGuard&, const TEvent&, bool result)
    {
        printf("[%s][guard] %s %s %s\n", sml::aux::get_type_name<SM>(), sml::aux::get_type_name<TGuard>(),
               sml::aux::get_type_name<TEvent>(), (result ? "[OK]" : "[Reject]"));
    }

    template <class SM, class TAction, class TEvent>
    void log_action(const TAction&, const TEvent&)
    {
        printf("[%s][action] %s %s\n", sml::aux::get_type_name<SM>(), sml::aux::get_type_name<TAction>(),
               sml::aux::get_type_name<TEvent>());
    }

    template <class SM, class TSrcState, class TDstState>
    void log_state_change(const TSrcState& src, const TDstState& dst)
    {
        printf("[%s][transition] %s -> %s\n", sml::aux::get_type_name<SM>(), src.c_str(), dst.c_str());
    }
};

namespace MyEvents
{
    struct startAdvertising
    {
    };
    struct onAdvertisingEnd
    {
    };
    //struct endAdvertising
    //{
    //};
    struct onDisconnectionComplete
    {
    };
    struct onScanTimeout
    {
    };
    struct endScanning
    {
    };
    struct onAdvertisingReport
    {
        ble::AdvertisingReportEvent bleEvent;
    };
} // namespace MyEvents

namespace
{
    inline auto Init         = boost::sml::state<class Init>;
    inline auto Scanning     = boost::sml::state<class Scanning>;

    inline auto Disconnected = boost::sml::state<class Disconnected>;
    inline auto Connected    = boost::sml::state<class Connected>;
} // namespace

namespace
{

} // namespace

namespace
{    
    struct do_advertise
    {
        auto operator()(IBluetoothGAPApplication &application) const
        {
            application.StartAdvertising();
        }
    };

    struct do_end_advertising
    {
        auto operator()(IBluetoothGAPApplication &application) const
        {
            printf("About to call application.StopAdvertising()...\r\n");
            application.StopAdvertising();
        }
    };

    struct do_scan
    {
        auto operator()(IBluetoothGAPApplication &application) const
        {
            application.StartScanning();
        }
    };

    struct do_end_scanning
    {
        auto operator()(IBluetoothGAPApplication &application) const
        {
            application.StopScanning();
        }
    };

    struct do_connect
    {
        auto operator()(IBluetoothGAPApplication &application, const MyEvents::onAdvertisingReport& e) const
        {
            application.ConnectToCentralDevice(e.bleEvent);
        }
    };
} // namespace

struct StateMachine_t 
{
    auto operator()() const
    {
        using namespace boost::sml;
        
        // clang-format off
        return make_transition_table(
            * "Init"_s                       + event<MyEvents::startAdvertising> / do_advertise{}           = state<AdvertisingSubMachine_t>
            , state<AdvertisingSubMachine_t> + event<MyEvents::onDisconnectionComplete> / do_end_advertising{} = state<AdvertisingSubMachine_t>
            , state<AdvertisingSubMachine_t> + event<MyEvents::onAdvertisingEnd> / do_scan{}                = "Scanning"_s
            , "Scanning"_s                   + event<MyEvents::onAdvertisingReport> / do_connect{}          = "Scanning"_s
            , "Scanning"_s                   + event<MyEvents::onScanTimeout> / do_end_scanning{}           = "Scanning"_s
            , "Scanning"_s                   + event<MyEvents::onDisconnectionComplete> / do_end_scanning{} = "Scanning"_s
            , "Scanning"_s                   + event<MyEvents::endScanning> / do_advertise{}                = state<AdvertisingSubMachine_t>
        );
        // clang-format on
    }
};

using MooreFSM_t = sm<StateMachine_t, logger<DebugLogger_t> >;

DebugLogger_t      g_TheDebugLogger();
MooreFSM_t         g_TheFSM{g_TheDebugLogger};

void riseHandler()
{
    g_TheFSM.process_event(buttonPressed{});
}

int main()
{
    printf("\r\n\r\nMooreFSM-MbedOS Application - Beginning... \r\n\r\n");
#ifdef MBED_MAJOR_VERSION
    printf("Mbed OS version: %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
#endif
    printf("Built: %s, %s\n\n", __DATE__, __TIME__);
    
    g_ButtonOne.rise(&riseHandler);
    
    while (!stopBlinking) 
    {
        g_UserLED = !g_UserLED;
        ThisThread::sleep_for(BLINKING_RATE);
    }

    while (true) // Run forever.
    {          
        // Wait around, button interrupts will interrupt this!
        ThisThread::sleep_for(SLEEPING_RATE);
    }
    
    printf("\r\n\r\nMooreFSM-MbedOS - Exiting.\r\n\r\n");
}

#include "mbed.h"
#include "sml.hpp" // [Boost::ext].SML

#define BLINKING_RATE 500ms
#define SLEEPING_RATE 10s

using namespace boost::sml;
namespace sml = boost::sml;

InterruptIn g_ButtonOne(SW2);
DigitalOut  g_UserLED(LED1);

bool g_BlinkingFlag{false};

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

namespace
{
    struct buttonPressed
    {
    };
} // FSM Event(s) unnamed namespace.

namespace
{    
    struct do_light_off
    {
        auto operator()() const
        {
            g_UserLED = LED_OFF;
        }
    };

    struct do_light_on
    {
        auto operator()() const
        {
            g_UserLED = LED_ON;
        }
    };

    struct do_light_blink
    {
        auto operator()() const
        {
            while (g_BlinkingFlag) 
            {
                g_UserLED = !g_UserLED;
                ThisThread::sleep_for(BLINKING_RATE);
            }
        }
    };
} // FSM Actions unnamed namespace.

struct StateMachine_t 
{
    auto operator()() const
    {
        using namespace boost::sml;
        
        // clang-format off
        return make_transition_table(
            * "Off"_s   + event<buttonPressed> / do_light_on{}    = "On"_s
            , "On"_s    + event<buttonPressed> / do_light_blink{} = "Blink"_s
            , "Blink"_s + event<buttonPressed> / do_light_off{}   = "Off"_s
        );
        // clang-format on
    }
};

using MooreFSM_t = sm<StateMachine_t, logger<DebugLogger_t> >;

DebugLogger_t g_TheDebugLogger();
MooreFSM_t    g_TheFSM{g_TheDebugLogger};

void riseHandler()
{
    if (g_TheFSM.is("Blink"_s)) // TBD, Nuertey Odzeyem, or it rather "On"_s ????
    {
        // Signal to stop blinking the LED.
        g_BlinkingFlag = false;
    }
    
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

    while (true) // Run forever.
    {          
        // Wait around, button interrupts will awaken us when they occur.
        ThisThread::sleep_for(SLEEPING_RATE);
    }
    
    printf("\r\n\r\nMooreFSM-MbedOS - Exiting.\r\n\r\n");
}

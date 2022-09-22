#include "mbed.h"
#include "events/mbed_events.h"
#include "sml.hpp" // [Boost::ext].SML

#define LED_ON  1
#define LED_OFF 0

#define BLINKING_RATE 500ms

using namespace boost::sml;
namespace sml = boost::sml;

// Push-button B1 USER: the user button is connected to the I/O PC13 by 
// default of the STM32 microcontroller.
InterruptIn g_UserButton(BUTTON1); 
DigitalOut  g_LEDOne(LED1);

EventQueue * g_pUserContextEventQueue = mbed_event_queue();

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
            g_LEDOne = LED_OFF;
        }
    };

    struct do_light_on
    {
        auto operator()() const
        {
            g_LEDOne = LED_ON;
        }
    };

    struct do_light_blink
    {
        auto operator()() const
        {
            while (g_BlinkingFlag) 
            {
                g_LEDOne = !g_LEDOne;
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

DebugLogger_t g_TheDebugLogger;
MooreFSM_t    g_TheFSM{g_TheDebugLogger};

void processEvent()
{
    g_TheFSM.process_event(buttonPressed{});
}

void riseHandler()
{
    if (g_TheFSM.is("On"_s))
    {
        // Signal to start blinking the LED.
        g_BlinkingFlag = true;
    }
    else if (g_TheFSM.is("Blink"_s))
    {
        // Signal to stop blinking the LED.
        g_BlinkingFlag = false;
    }
    
    // Transform processing from interrupt context to the main user 
    // thread context by means of the shared event queue.
    if (g_pUserContextEventQueue)
    {
        g_pUserContextEventQueue->call(processEvent);
    }
    else
    {
        printf("FATAL! g_pUserContextEventQueue() is nullptr during riseHandler()!\r\n");
    }
}

int main()
{
    printf("\r\n\r\nMooreFSM-MbedOS Application - Beginning... \r\n\r\n");
#ifdef MBED_MAJOR_VERSION
    printf("Mbed OS version: %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
#endif
    printf("Built: %s, %s\n\n", __DATE__, __TIME__);
    
    g_UserButton.rise(&riseHandler);

    // Setup complete, so we now dispatch the shared queue from main().
    // Events are executed by the dispatch_forever() method which never 
    // returns.
    g_pUserContextEventQueue->dispatch_forever();
    
    printf("\r\n\r\nMooreFSM-MbedOS - Exiting.\r\n\r\n");
}

#include "usonic.h"

#include <pigpio.h>
#include <thread>
#include "gpio_init.h"
#include "vlog.h"

using namespace std;
using namespace std::chrono_literals;

//=======================================================================================
USonic::USonic(int trig, int echo)
    : leg_trig( trig )
    , leg_echo( echo )
{
    static GPIO_Init _;

    gpioSetMode( leg_trig, PI_OUTPUT );
    gpioSetMode( leg_echo, PI_INPUT  );
    gpioSetAlertFuncEx( leg_echo, &USonic::on_alert_ex, this );
}
//=======================================================================================
USonic::~USonic()
{
    gpioSetAlertFuncEx( leg_echo, nullptr, nullptr );
}
//=======================================================================================
void USonic::trigger()
{
    cur_wait = WaitOn;
    gpioWrite( leg_trig, PI_ON );
    gpioDelay( 10 );
    gpioWrite( leg_trig, PI_OFF );
}
//=======================================================================================
void USonic::on_alert( int gpio, int level, uint32_t tick )
{
    if ( gpio != leg_echo )
    {
        vwarning << "bad leg";
        return;
    }
    if ( cur_wait == WaitOn && level == 1 )
    {
        cur_wait = WaitOff;
        start_tick = tick;
        return;
    }
    if ( cur_wait == WaitOff && level == 0 )
    {
        cur_wait = Bad;
        auto ticks = double(tick) - double(start_tick);
        measured( ticks/58.2, start_tick, tick );
        return;
    }
    vwarning << "Bad measure:" << gpio << level << cur_wait;
    measured(0,0,0);
}
//=======================================================================================
void USonic::on_alert_ex(int gpio, int level, uint32_t tick, void *userdata)
{
    auto self = static_cast<USonic*>(userdata);
    self->on_alert( gpio, level, tick );
}
//=======================================================================================

//=======================================================================================
USonicSync::USonicSync( USonic *sonic_ )
    : sonic(sonic_)
{
    sonic->measured += [this](auto val, auto, auto)
    {
        this->val = val;
        this->measured = true;
    };
}
//=======================================================================================
double USonicSync::measure()
{
    measured = false;
    sonic->trigger();

    while ( !measured )
    {
        this_thread::sleep_for( 2500us );
    }

    this_thread::sleep_for( 5ms );
    return val;
}
//=======================================================================================

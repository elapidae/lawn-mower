#include <pigpio.h>
#include <thread>
#include "vlog.h"
#include "vtime_meter.h"
#include "engine.h"

using namespace std;
using namespace std::chrono_literals;

#define TRIG 15
#define ECHO 14

//=======================================================================================
uint32_t inited_tick;
uint32_t start_tick, end_tick;
bool has_read = false;
void echo_alert( int gpio, int level, uint32_t tick )
{
    uint32_t diffTick;

    // make sure we don't measure trigger pulse
    if ( tick <= inited_tick ) return;

    if ( level == PI_ON ) // start counting on rising edge
    {
        start_tick = tick;
        return;
    }

    if ( level != PI_OFF ) // stop counting on falling edge
    {
        vwarning << "Strange level: " << level;
        return;
    }

    // Now level == PI_OFF
    end_tick = tick;
    auto diff = (end_tick - start_tick) / 58;

    has_read = true;
    vdeb << "tick:" << tick << ", len:" << diff;
}
//=======================================================================================
int test_UZ()
{
    vdeb << "gpio initialise";
    if (gpioInitialise() < 0) throw verror << "gpioInitialise";

    vdeb << "set modes";
    gpioSetMode( TRIG, PI_OUTPUT );
    gpioSetMode( ECHO, PI_INPUT  );
    gpioSetAlertFunc( ECHO, echo_alert );

    vtime_meter tm;
    int i = 0;
    //for (int i = 0; i < 300; ++i)
    while(1)
    {
        has_read = false;

        gpioWrite(TRIG, PI_ON);
        gpioDelay(10);
        gpioWrite(TRIG, PI_OFF);

        inited_tick = gpioTick(); // get tick right after sending pulse

        while ( !has_read )
        {
            std::this_thread::sleep_for( 1us );
        }
        std::this_thread::sleep_for( 50ms );
        //vdeb << "finished" << i++ << tm.restart().ms();
    }

    vdeb << "about to terminate";
    gpioTerminate();
    return 0;
}
//=======================================================================================

volatile uint32_t before;  // used for sonar distance

int last_range = 0;	     // last sonar reading

//=======================================================================================
void sleep( int t )
{
    gpioSleep( PI_TIME_RELATIVE, t, 0 );
}
//=======================================================================================
// delay in miliseconds
//void delay(int ms)
//{
//    gpioDelay( 1000 * ms );
//}
//=======================================================================================
void range(int gpio, int level, uint32_t tick)
{
    static uint32_t startTick, endTick;

    uint32_t diffTick;

    // make sure we don't measure trigger pulse
    if ( tick <= before ) return;

    if (level == PI_ON) // start counting on rising edge
    {
        startTick = tick;
        return;
    }

    if (level != PI_OFF) // stop counting on falling edge
    {
        return;
    }

    // Now level == PI_OFF
    endTick = tick;
    diffTick = (endTick - startTick) / 58;
    last_range = diffTick;

    if (diffTick < 600)
    {
        vdeb << diffTick;
    }
    else
    {
        vdeb << "OUT OF RANGE"; // for seeedstudio sensor
        last_range = 0;
    }
}
//=======================================================================================
// send out an ultrasonic 'ping'
void ping(void)
{
    before = 0xffffffff; // set for guard variable

    gpioSetMode(TRIG, PI_OUTPUT);

    // trigger a sonar pulse

    gpioWrite(TRIG, PI_OFF);
    gpioDelay(5);
    gpioWrite(TRIG, PI_ON);
    gpioDelay(10);
    gpioWrite(TRIG, PI_OFF);
    gpioDelay(5);

    gpioSetMode(ECHO, PI_INPUT);

    before = gpioTick(); // get tick right after sending pulse
}
//=======================================================================================
int testing_legs_grom();
int main()
{
    //-------------------------
    return Engine::test();
    //-------------------------
    return test_UZ();
    //-------------------------
    return testing_legs_grom();
    //-------------------------

    vdeb << "gpio initialise";
    if (gpioInitialise() < 0) throw verror << "gpioInitialise";

    vdeb << "set mode ECCHO <- input";
    gpioSetMode(ECHO, PI_INPUT);

    // register callback on change of sonar pin
    vdeb << "set alert 'range' func";
    gpioSetAlertFunc(ECHO, range);

    for (int i = 0; i < 5; ++i)
    {
        ping();	// prime the last_range variable
        sleep(1);
        vdeb << "round" << i << ": last range:" << last_range;
    }


    vdeb << "about to terminate";
    gpioTerminate();
    return 0;
}
//=======================================================================================
int testing_legs_grom()
{
    if (gpioInitialise() < 0) throw verror << "gpioInitialise";

    int cnt = 0;
    for (int i = 0; i < 5; ++i)
    {
        vdeb << ++cnt << "input";
        gpioSetMode(ECHO, PI_INPUT);
        this_thread::sleep_for(1s);

        vdeb << ++cnt << "output";
        gpioSetMode(ECHO, PI_OUTPUT);

        for (int i = 0; i < 400; ++i)
        {
            vdeb << i << "on";
            gpioWrite(TRIG, PI_ON);
            this_thread::sleep_for(1s);
            vdeb << i << "off";
            gpioWrite(TRIG, PI_OFF);
            this_thread::sleep_for(1s);
        }
    }

    gpioTerminate();

    return 0;
}
//=======================================================================================

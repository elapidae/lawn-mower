#include "engine.h"

#include <pigpio.h>
#include <thread>
#include "vlog.h"
using namespace std;
using namespace chrono_literals;

//=======================================================================================
/*
static void echo_alert( int gpio, int level, uint32_t tick )
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
*/
//=======================================================================================
int Engine::test()
{
    if (gpioInitialise() < 0) throw verror << "gpioInitialise";

    int m_ZF_Direction  = 11;  	// ZF - digital direction motor 0-5v (high/low)
    int m_EL_Start_Stop = 5;  	// EL - digital break motor 0-5v (high/low)
    int m_VR_speed      = 0;   	// VR - analog speed control motor 0-255 (int)
    int m_M_Speed_Pulse = 24; 	// M- Speed Pulse Motor Left

    gpioSetMode( m_EL_Start_Stop, PI_OUTPUT );//stop/start - EL
    gpioSetMode( m_ZF_Direction,  PI_OUTPUT ); //direction  - ZF

    gpioWrite( m_EL_Start_Stop, PI_OFF );
    gpioWrite( m_ZF_Direction,  PI_ON  );
    //gpioWrite( m_ZF_Direction,  PI_OFF );

    for ( int i = 0; i < 256; ++i )
    {
        gpioPWM( m_VR_speed, 255 );
        this_thread::sleep_for( 50ms );
    }
    for ( int i = 255; i >= 0; --i )
    {
        gpioPWM( m_VR_speed, i );
        this_thread::sleep_for( 50ms );
    }

    gpioPWM( m_VR_speed, 0 );
    gpioWrite( m_EL_Start_Stop, PI_ON );
    gpioTerminate();
    return 0;
}

Engine::Engine()
{

}

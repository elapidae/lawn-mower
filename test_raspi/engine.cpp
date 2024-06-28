#include "engine.h"
#include <pigpio.h>
#include <thread>
#include "vlog.h"
#include "stdio.h"
#include <unistd.h>
#include "gpio_init.h"

using namespace std;
using namespace chrono_literals;

//=======================================================================================
Engine::Engine( int dir, int start_stop, int spd, int holl )
    : leg_direction( dir )
    , leg_start_stop( start_stop )
    , leg_speed( spd )
    , leg_holl( holl )
{
    static GPIO_Init _;

    gpioSetMode( leg_direction,   PI_OUTPUT );
    gpioSetMode( leg_start_stop,  PI_OUTPUT );
    gpioSetMode( leg_speed,       PI_OUTPUT );
    gpioSetMode( leg_holl,        PI_INPUT  );
    gpioSetAlertFuncEx( leg_holl, &Engine::on_alert_ex, this );
}
//=======================================================================================
Engine::~Engine()
{
    gpioSetAlertFuncEx( leg_holl, nullptr, nullptr );
    set_speed(0);
    stop();
}
//=======================================================================================
void Engine::on_alert_ex( int gpio, int level, uint32_t tick, void *self )
{
    auto ptr = static_cast<Engine*>( self );
    ptr->on_alert( gpio, level, tick );
}
//=======================================================================================
void Engine::on_alert( int gpio, int level, uint32_t tick )
{
    if ( !cur_alert ) return;
    (this->*cur_alert)( gpio, level, tick );
}
//=======================================================================================
void Engine::set_direction( int dir )
{
    gpioWrite( leg_direction, dir );
}
//=======================================================================================
void Engine::set_speed( int spd )
{
    gpioPWM( leg_speed, spd );
}
//=======================================================================================
void Engine::set_stop( bool stop )
{
    auto signal = stop ? PI_ON : PI_OFF;
    gpioWrite( leg_start_stop, signal );
    stopped = stop;
}
//=======================================================================================
void Engine::move_until_steps( int dir, int spd, int steps )
{
    cur_alert = &Engine::on_stop_by_steps;
    set_direction( dir );
    set_speed( spd );
    steps_until_stop = steps;
    run();
}
//=======================================================================================
void Engine::on_stop_by_steps( int gpio, int level, uint32_t tick )
{
    if ( gpio != leg_holl )
    {
        vwarning << "gpio not hall";
        return;
    }
    vdeb << "on steps" << steps_until_stop;
    if ( --steps_until_stop > 0 )
    {
        return;
    }

    stop();
}
//=======================================================================================


//=======================================================================================
static void echo_alert( int gpio, int level, uint32_t tick )
{
    static int counter = 0;
    vdeb << counter++ << gpio << level << tick;
}
//=========================
int Engine::test()
{
    if (gpioInitialise() < 0) throw verror << "gpioInitialise";
//Left Motor
    int ml_ZF_Direction  = 11;  	// ZF - digital direction motor 0-5v (high/low)
    int ml_EL_Start_Stop = 5;       // EL - digital break motor 0-5v (high/low)
    int ml_VR_speed      = 0;   	// VR - analog speed control motor 0-255 (int)
    int ml_M_Speed_Pulse = 24;      // M- Speed Pulse Motor Left
//Right Motor
    int mr_ZF_Direction  = 8;       // ZF - digital direction motor 0-5v (high/low)
    int mr_EL_Start_Stop = 6;       // EL - digital break motor 0-5v (high/low)
    int mr_VR_speed      = 1;   	// VR - analog speed control motor 0-255 (int)
    int mr_M_Speed_Pulse = 25;      // M- Speed Pulse Motor Right

    gpioSetMode( ml_EL_Start_Stop, PI_OUTPUT ); //stop/start - EL
    gpioSetMode( ml_ZF_Direction,  PI_OUTPUT ); //direction  - ZF
    gpioSetMode( ml_M_Speed_Pulse, PI_INPUT);   //Pulse motor
    gpioSetAlertFunc( ml_M_Speed_Pulse, echo_alert );

    gpioSetMode( mr_EL_Start_Stop, PI_OUTPUT ); //stop/start - EL
    gpioSetMode( mr_ZF_Direction,  PI_OUTPUT ); //direction  - ZF

    gpioWrite( ml_EL_Start_Stop, PI_OFF );
    gpioWrite( ml_ZF_Direction,  PI_ON  );
    //gpioWrite( m_ZF_Direction,  PI_OFF );

    gpioWrite( mr_EL_Start_Stop, PI_OFF );
    gpioWrite( mr_ZF_Direction,  PI_OFF  );

    //gpioPWM( ml_VR_speed, 255 );
    //gpioPWM( mr_VR_speed, 255 );
    //this_thread::sleep_for( 300ms );
    gpioPWM( ml_VR_speed, 0 );
    gpioPWM( mr_VR_speed, 0 );
    //int step = 0;
    while (1)
    {
        //vdeb << step++;
        this_thread::sleep_for( 30ms );
         //--step;
    }

 //   for ( int i = 150; i < 256; ++i )
 //   {
 //       gpioPWM( ml_VR_speed, 255 );
 //       gpioPWM( mr_VR_speed, 255 );
 //       this_thread::sleep_for( 50ms );
 //       printf("Speed motor=%d\n",i);
 //       a = gpioRead(ml_M_Speed_Pulse);
 //       if(a=1){ ++b;};
 //   }
 //   printf("Count step motor=%d\n", b);
 //   for ( int i = 255; i >= 0; --i )
 //   {
 //       gpioPWM( ml_VR_speed, i );
 //       gpioPWM( mr_VR_speed, i );
 //       this_thread::sleep_for( 50ms );
 //   }

    gpioPWM( ml_VR_speed, 0 );
    gpioPWM( mr_VR_speed, 0 );
    gpioWrite( ml_EL_Start_Stop, PI_ON );
    gpioWrite( mr_EL_Start_Stop, PI_ON );
    this_thread::sleep_for( 70ms );
    gpioWrite( ml_EL_Start_Stop, PI_OFF );
    gpioWrite( mr_EL_Start_Stop, PI_OFF );
    gpioTerminate();
    return 0;
}

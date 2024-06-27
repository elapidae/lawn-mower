#include "engine.h"
#include <pigpio.h>
#include <thread>
#include "vlog.h"
#include "stdio.h"
#include <unistd.h>
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

Engine::Engine()
{

}

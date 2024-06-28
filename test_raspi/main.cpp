#include <pigpio.h>
#include <thread>
#include "vlog.h"
#include "vtime_meter.h"
#include "engine.h"
#include "gpio_init.h"
#include "usonic.h"

using namespace std;
using namespace std::chrono_literals;

//=======================================================================================
int main()
{
    //-------------------------
    auto engine = Engine::right_engine();
    //auto engine = Engine::left_engine();
    //while( 1 ) {this_thread::sleep_for(10ms); }

    engine.move_until_steps( 0, 50, 90 );
    while( !engine.stopped ) {this_thread::sleep_for(10ms); }

    this_thread::sleep_for(100ms);

    engine.move_until_steps( 1, 50, 90 );
    while( !engine.stopped ) {this_thread::sleep_for(10ms); }

    return 0;
    //-------------------------
    return Engine::test();
    //-------------------------

    this_thread::sleep_for(1s);

    return 0;
}
//=======================================================================================

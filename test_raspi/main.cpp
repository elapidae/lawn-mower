#include <pigpio.h>
#include <thread>
#include "vlog.h"
#include "vtime_meter.h"
#include "engine.h"
#include "gpio_init.h"
#include "usonic.h"
#include <fstream>
#include "vcat.h"

using namespace std;
using namespace std::chrono_literals;

//=======================================================================================
void test_round()
{
    auto R = Engine::right_engine();
    auto L = Engine::left_engine();

    auto _s = USonic::first();
    auto S = USonicSync( &_s );


    auto spd = 40;
    auto holl = 250;
    ofstream f;
    f.open( vcat("../", spd, "-", holl, ".txt") );

    R.move_until_steps( 1, spd, holl );
    L.move_until_steps( 1, spd, holl );
    while( !L.stopped && !R.stopped )
    {
        auto dist = S.measure();
        f << dist << ";" << L.steps_until_stop << ";" << R.steps_until_stop << "\n";
        vdeb << dist;
    }
}
int main()
{
    //-------------------------
    test_round();
    return 0;
    //-------------------------
    this_thread::sleep_for(1s);

    return 0;
}
//=======================================================================================

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

    auto _s1 = USonic::first();
    auto _s2 = USonic::second();
    auto _s3 = USonic::three();
    auto _s4 = USonic::four();

    auto S1 = USonicSync( &_s1 );
    auto S2 = USonicSync( &_s2 );
    auto S3 = USonicSync( &_s3 );
    auto S4 = USonicSync( &_s4 );


    auto spd = 38;
    auto holl = 110*5;
    ofstream f;
    f.open( vcat("../", spd, "-", holl, ".txt") );

    R.move_until_steps( 1, spd, holl );
    L.move_until_steps( 1, spd, holl );
    while( !L.stopped && !R.stopped )
    {
        auto d1 = S1.measure();
        auto d2 = S2.measure();
        auto d3 = S3.measure();
        auto d4 = S4.measure();
        f << d1 << d2 << d3 << d4
          << "|" << L.steps_until_stop << ";" << R.steps_until_stop << "\n";
        //vdeb << d1 << d2 << d3 << d4;
    }
}
int main()
{
    //-------------------------
    test_round();
    return 0;
    //-------------------------
    auto _s1 = USonic::first();
    auto _s2 = USonic::second();
    auto _s3 = USonic::three();
    auto _s4 = USonic::four();
    auto S1 = USonicSync( &_s1 );
    auto S2 = USonicSync( &_s2 );
    auto S3 = USonicSync( &_s3 );
    auto S4 = USonicSync( &_s4 );
    vdeb << S1.measure();
    vdeb << S2.measure();
    vdeb << S3.measure();
    vdeb << S4.measure();
    return 0;
    //-------------------------
    this_thread::sleep_for(1s);

    return 0;
}
//=======================================================================================

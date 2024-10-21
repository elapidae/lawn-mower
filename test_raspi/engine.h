#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>
#include "vsignal.h"

class Engine final
{
public:
    static int test();
    static int test_holl();

    static Engine left_engine()  { return {11, 5, 0, 24}; }
    static Engine right_engine() { return { 8, 6, 1, 25}; }


    Engine( int dir, int start_stop, int spd, int holl );
    ~Engine();

    void set_direction( int dir );
    void set_speed( int spd );

    void set_stop( bool stop );
    void stop() { set_stop(true);  }
    void run()  { set_stop(false); }

    void move_until_steps( int dir, int spd, int steps );

    bool stopped = true;
    int steps_until_stop = 0;

    void set_holl_signal();
    vsignal<int, uint32_t> holl_signal;

private:
    int leg_direction   = -1;
    int leg_start_stop  = -1;
    int leg_speed       = -1;
    int leg_holl        = -1;

    void on_stop_by_steps( int gpio, int level, uint32_t tick );
    void on_holl_signal( int gpio, int level, uint32_t tick );

    using CurAlertCallBack = void(Engine::*)( int gpio, int level, uint32_t tick );
    CurAlertCallBack cur_alert = nullptr;

    void on_alert( int gpio, int level, uint32_t tick );
    static void on_alert_ex( int gpio, int level, uint32_t tick, void * userdata );
};

#endif // ENGINE_H

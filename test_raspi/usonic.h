#ifndef USONIC_H
#define USONIC_H

#include "vsignal.h"

class USonic final
{
public:
    //initiz ultrasonic
    static USonic first() { return {15, 14}; }
    static USonic second() { return {17, 27}; }
    static USonic three() { return {22, 23}; }
    static USonic four() { return {10, 9}; }

    USonic( int trig, int echo );
    ~USonic();

    void trigger();
    vsignal<double, uint32_t, uint32_t> measured;

private:
    int leg_trig, leg_echo;

    uint32_t start_tick;
    enum { Bad, WaitOn, WaitOff } cur_wait = Bad;

    void on_alert( int gpio, int level, uint32_t tick );
    static void on_alert_ex( int gpio, int level, uint32_t tick, void * userdata );

    USonic(const USonic&) = delete;
    USonic(USonic&&) = delete;
};

class USonicSync
{
public:
    USonicSync( USonic *sonic );

    double measure();

private:
    USonic *sonic;
    bool measured;
    double val;
};

#endif // USONIC_H

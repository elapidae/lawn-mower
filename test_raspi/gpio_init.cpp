#include "gpio_init.h"
#include <pigpio.h>
#include "vlog.h"

//=======================================================================================
class GPIO_Init::pimpl
{
public:
    pimpl()
    {
        if ( gpioInitialise() < 0 )
            throw verror << "gpioInitialise";
        vtrace << "gpio inited";
    }

    ~pimpl()
    {
        gpioTerminate();
        vtrace << "gpio terminated";
    }

    static std::shared_ptr<pimpl> inst()
    {
        static std::shared_ptr<pimpl> res( new pimpl );
        return res;
    }
};
//=======================================================================================
GPIO_Init::GPIO_Init()
    : p( pimpl::inst() )
{}
//=======================================================================================
GPIO_Init::~GPIO_Init()
{}
//=======================================================================================

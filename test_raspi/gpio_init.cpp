#include "gpio_init.h"
#include <pigpio.h>
#include "vlog.h"

//=======================================================================================
class GPIO_Init::pimpl
{
public:
    pimpl()
    {
        auto rcode = gpioInitialise();
        if ( rcode < 0 )
            throw verror << "gpioInitialise: " << rcode;
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

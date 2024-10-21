#include "gpio_init.h"
#include <pigpio.h>
#include "vlog.h"

#include <vector>

//=======================================================================================
class GPIO_Init::pimpl
{
public:

    std::vector<exit_fn> exit_funcs;

    pimpl()
    {
        auto rcode = gpioInitialise();
        if ( rcode < 0 )
            throw verror << "gpioInitialise: " << rcode;

        vtrace << "gpio inited";
    }

    ~pimpl()
    {
        for ( auto && fn: exit_funcs )
            fn();

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
void GPIO_Init::add_exit_fn( exit_fn fn )
{
    p->exit_funcs.push_back( fn );
}
//=======================================================================================

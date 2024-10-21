#ifndef GPIO_INIT_H
#define GPIO_INIT_H

#include <memory>
#include <functional>

//=======================================================================================
class GPIO_Init final
{
public:    
    GPIO_Init();
    ~GPIO_Init();

    using exit_fn = std::function<void()>;

    void add_exit_fn( exit_fn fn );

private:
    class pimpl; std::shared_ptr<pimpl> p;
};
//=======================================================================================

#endif // GPIO_INIT_H

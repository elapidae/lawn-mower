#ifndef GPIO_INIT_H
#define GPIO_INIT_H

#include <memory>

//=======================================================================================
class GPIO_Init final
{
public:    
    GPIO_Init();
    ~GPIO_Init();

private:
    class pimpl; std::shared_ptr<pimpl> p;
};
//=======================================================================================

#endif // GPIO_INIT_H

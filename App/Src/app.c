#include "app.h"

void App()
{
    while (1)
    {
        LOG_INFO("Hello World! %d %d", SYS_MS, SYS_US);
        HAL_Delay(1);
    }
}
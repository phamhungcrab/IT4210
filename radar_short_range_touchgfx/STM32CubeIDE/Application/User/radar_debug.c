#include "radar_debug.h"
#include "main.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart1;

void RadarDebug_Printf(const char *fmt, ...)
{
    char buffer[192];

    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    if (len <= 0)
    {
        return;
    }

    if (len > (int)(sizeof(buffer) - 1))
    {
        len = sizeof(buffer) - 1;
    }

    HAL_UART_Transmit(&huart1, (uint8_t *)buffer, (uint16_t)len, 50);
}

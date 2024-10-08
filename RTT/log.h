#ifndef LOG_H
#define LOG_H
#include <string.h>

#include "SEGGER_RTT.h"
#include "stm32f4xx.h"

#define HAL_GetUs() ({                   \
    uint32_t tms = SysTick->LOAD + 1;    \
    ((tms - SysTick->VAL) * 1000 / tms); \
})

#define HAL_Delay(Delay) ({                     \
    uint32_t tickstart = HAL_GetTick();         \
    while ((HAL_GetTick() - tickstart) < Delay) \
    {                                           \
    }                                           \
})

extern uint32_t HAL_GetTick(void);
#define SYS_MS HAL_GetTick()
#define SYS_US HAL_GetUs()

#define USE_LOG_DEBUG 1
#define PRINT_TIMESTAMP 1

#define LOG_ARR_BYTE1 "%02x "
#define LOG_ARR_BYTE2 "%04x "
#define LOG_ARR_BYTE4 "%08x "
#define LOG_ARR_FLOAT "%f "
#define LOG_ARR_INT "%d "
#define LOG_ARR_CHAR "%c"

#if USE_LOG_DEBUG
#if PRINT_TIMESTAMP
#define LOG_PROTO(type, color, terminal, format, ...)                                                                 \
    SEGGER_RTT_SetTerminal(terminal);                                                                                 \
    SEGGER_RTT_printf(0, "[%02d:%03d:%03d] %s%s" format "%s\r\n", (SYS_MS / 1000) % 60, SYS_MS % 1000, SYS_US % 1000, \
                      color, type, ##__VA_ARGS__, RTT_CTRL_RESET);                                                    \
    SEGGER_RTT_SetTerminal(0);
#else
#define LOG_PROTO(type, color, terminal, format, ...)                                         \
    SEGGER_RTT_SetTerminal(terminal);                                                         \
    SEGGER_RTT_printf(0, "%s%s" format "%s\r\n", color, type, ##__VA_ARGS__, RTT_CTRL_RESET); \
    SEGGER_RTT_SetTerminal(0);
#endif

// 修改后的日志宏，增加了终端参数
#define LOG_INFO_T(terminal, format, ...) LOG_PROTO("[INFO]:", "", terminal, format, ##__VA_ARGS__)                           // 无颜色日志输出
#define LOG_DEBUG_T(terminal, format, ...) LOG_PROTO("[DEBUG]:", RTT_CTRL_TEXT_BRIGHT_GREEN, terminal, format, ##__VA_ARGS__) // 绿色日志输出
#define LOG_WARN_T(terminal, format, ...) LOG_PROTO("[WARN]:", RTT_CTRL_TEXT_BRIGHT_YELLOW, terminal, format, ##__VA_ARGS__)  // 黄色日志输出
#define LOG_ERROR_T(terminal, format, ...) LOG_PROTO("[ERROR]:", RTT_CTRL_TEXT_BRIGHT_RED, terminal, format, ##__VA_ARGS__)   // 红色日志输出

// 兼容不带终端参数的版本，默认使用终端 0
#define LOG_INFO(format, ...) LOG_INFO_T(0, format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) LOG_DEBUG_T(0, format, ##__VA_ARGS__)
#define LOG_WARN(format, ...) LOG_WARN_T(0, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) LOG_ERROR_T(0, format, ##__VA_ARGS__)

#define LOG_CLEAR() SEGGER_RTT_WriteString(0, "\r\n" RTT_CTRL_CLEAR) // 清屏

// 打印数组
#define LOG_ARRAY(terminal, pArr, len, logArrayType)                                    \
    do                                                                                  \
    {                                                                                   \
        SEGGER_RTT_SetTerminal(terminal);                                               \
        char addNewLine = 1;                                                            \
        if (strcmp(logArrayType, LOG_ARR_BYTE1) == 0)                                   \
        {                                                                               \
            for (int i = 0; i < len; i++)                                               \
                SEGGER_RTT_printf(0, logArrayType, *((uint8_t *)pArr + i));             \
        }                                                                               \
        else if (strcmp(logArrayType, LOG_ARR_BYTE2) == 0)                              \
        {                                                                               \
            for (int i = 0; i < len; i++)                                               \
                SEGGER_RTT_printf(0, logArrayType, *((uint16_t *)pArr + i));            \
        }                                                                               \
        else if (strcmp(logArrayType, LOG_ARR_BYTE4) == 0)                              \
        {                                                                               \
            for (int i = 0; i < len; i++)                                               \
                SEGGER_RTT_printf(0, logArrayType, *((uint32_t *)pArr + i));            \
        }                                                                               \
        else if (strcmp(logArrayType, LOG_ARR_FLOAT) == 0)                              \
        {                                                                               \
            for (int i = 0; i < len; i++)                                               \
                SEGGER_RTT_printf(0, logArrayType, *((float *)pArr + i));               \
        }                                                                               \
        else if (strcmp(logArrayType, LOG_ARR_INT) == 0)                                \
        {                                                                               \
            for (int i = 0; i < len; i++)                                               \
                SEGGER_RTT_printf(0, logArrayType, *((int *)pArr + i));                 \
        }                                                                               \
        else if (strcmp(logArrayType, LOG_ARR_CHAR) == 0)                               \
        {                                                                               \
            for (int i = 0; i < len; i++)                                               \
                SEGGER_RTT_printf(0, logArrayType, *((char *)pArr + i));                \
            if (*((char *)pArr + len - 1) == '\r' || *((char *)pArr + len - 1) == '\n') \
                addNewLine = 0;                                                         \
        }                                                                               \
        if (addNewLine)                                                                 \
            SEGGER_RTT_printf(0, "\r\n");                                               \
        SEGGER_RTT_SetTerminal(0);                                                      \
    } while (0)

#define LOG_INIT()               \
    do                           \
    {                            \
        SEGGER_RTT_Init();       \
        LOG_CLEAR();             \
        LOG_INFO("RTT Inited!"); \
    } while (0)

#else
#define LOG_INFO(format, ...)
#define LOG_DEBUG(format, ...)
#define LOG_WARN(format, ...)
#define LOG_ERROR(format, ...)
#define LOG_CLEAR()
#define LOG_ARRAY(pArr, len, logArrayType, terminal)
#define LOG_INIT()
#endif

#endif

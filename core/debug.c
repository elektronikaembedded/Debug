/****************************************************************************************
 * @file        debug.c
 * @author      Sarath S
 * @date        2026-01-02
 * @version     1.0
 * @brief       Debug and logging module implementation
 *
 * @details
 * Implements the debug framework.
 * Supports log levels, formatted logging, sequence numbers, timestamps,
 * and thread-safe output through abstracted transport and port layers.
 *
 * @contact     elektronikaembedded@gmail.com
 * @website     https://elektronikaembedded.wordpress.com
 ****************************************************************************************/

/****************************** Header include files ************************************/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "config.h"
#include "debug.h"
#include "debug_transport.h"
#include "debug_port.h"

/****************************** Type definitions ****************************************/
/**
 * @brief Debug runtime context structure
 */
typedef struct
{
    const debug_transport_hal_t *transport; /**< Active transport HAL */
    const debug_port_t          *debug_port; /**< OS / platform port */
    log_level_t                  level;      /**< Current log level */
    uint8_t                      initialized;/**< Init state */
} debug_context_t;

/****************************** Static variables ****************************************/
static debug_context_t debug_ctx = {0};
static uint32_t log_sequence_no = 0;
static char s_buffer[DEBUG_BUFFER_SIZE];

/****************************** Static functions ****************************************/
/**
 * @brief Generate next log sequence number (thread-safe via port layer)
 */
static uint32_t debug_next_sequence(void)
{
    uint32_t seq;

    if (debug_ctx.debug_port &&
        debug_ctx.debug_port->debug_port_ops.lock)
    {
        debug_ctx.debug_port->debug_port_ops.lock();
    }

    seq = ++log_sequence_no;

    if (debug_ctx.debug_port &&
        debug_ctx.debug_port->debug_port_ops.unlock)
    {
        debug_ctx.debug_port->debug_port_ops.unlock();
    }

    return seq;
}

/****************************** Public functions ***************************************/
int debug_init(const debug_transport_hal_t *trns_hal,
               const debug_port_t *debug_port)
{
    if ((NULL == trns_hal) || (NULL == debug_port))
    {
        return -1;
    }

    debug_ctx.transport   = trns_hal;
    debug_ctx.debug_port  = debug_port;
    debug_ctx.level       = LOG_DEBUG;
    debug_ctx.initialized = 0;

    if (debug_ctx.transport->debug_transport_ops.init)
    {
        if (0 != debug_ctx.transport->debug_transport_ops.init())
        {
            return -1;
        }
    }

    if (debug_ctx.debug_port->debug_port_ops.init)
    {
        if (0 != debug_ctx.debug_port->debug_port_ops.init())
        {
            return -1;
        }
    }

    debug_ctx.initialized = 1;
    return 0;
}

void debug_set_level(log_level_t level)
{
    debug_ctx.level = level;
}

log_level_t debug_get_level(void)
{
    return debug_ctx.level;
}

int debug_write(const char *str)
{
    if ((0 == debug_ctx.initialized) ||
        (NULL == debug_ctx.transport) ||
        (NULL == debug_ctx.transport->debug_transport_ops.write) ||
        (NULL == str))
    {
        return -1;
    }

    if (debug_ctx.debug_port &&
        debug_ctx.debug_port->debug_port_ops.lock)
    {
        debug_ctx.debug_port->debug_port_ops.lock();
    }

    int ret = debug_ctx.transport->debug_transport_ops.write(
        (const uint8_t *)str, strlen(str));

    if (debug_ctx.debug_port &&
        debug_ctx.debug_port->debug_port_ops.unlock)
    {
        debug_ctx.debug_port->debug_port_ops.unlock();
    }

    return ret;
}

int debug_printf(const char *fmt, ...)
{
    if ((0 == debug_ctx.initialized) || (NULL == fmt))
    {
        return -1;
    }

    va_list args;
    va_start(args, fmt);

    int len = vsnprintf(s_buffer, sizeof(s_buffer), fmt, args);
    va_end(args);

    if (len <= 0)
    {
        return len;
    }

    if ((size_t)len >= sizeof(s_buffer))
    {
        s_buffer[sizeof(s_buffer) - 1] = '\0';
    }

    return debug_write(s_buffer);
}

int debug_log(log_level_t level, const char *fmt, ...)
{
    if ((0 == debug_ctx.initialized) || (level > debug_ctx.level))
    {
        return 0; /* Filtered */
    }

    uint32_t ts = 0;
    uint32_t seq = 0;
    const char *thread = "MAIN";

#if DEBUG_ENABLE_TIME_DATE_INFO == YES
    if (debug_ctx.debug_port &&
        debug_ctx.debug_port->debug_port_ops.get_timestamp)
    {
        ts = debug_ctx.debug_port->debug_port_ops.get_timestamp();
    }
#endif

#if DEBUG_ENABLE_THREAD_INFO == YES
    if (debug_ctx.debug_port &&
        debug_ctx.debug_port->debug_port_ops.get_thread_name)
    {
        thread = debug_ctx.debug_port->debug_port_ops.get_thread_name();
    }
#endif

#if DEBUG_ENABLE_SEQUENCE_NO == YES
    seq = debug_next_sequence();
#endif

    const char *level_str = "LOG";
    if (level == LOG_ERROR) level_str = "ERROR";
    else if (level == LOG_WARN) level_str = "WARN";
    else if (level == LOG_INFO) level_str = "INFO";
    else if (level == LOG_DEBUG) level_str = "DEBUG";

    size_t n = 0;

#if DEBUG_ENABLE_SEQUENCE_NO == YES
    n += snprintf(&s_buffer[n], sizeof(s_buffer) - n, "[%05lu]", (unsigned long)seq);
#endif

#if DEBUG_ENABLE_TIME_DATE_INFO == YES
    n += snprintf(&s_buffer[n], sizeof(s_buffer) - n, "[%lu]", (unsigned long)ts);
#endif

#if DEBUG_ENABLE_THREAD_INFO == YES
    n += snprintf(&s_buffer[n], sizeof(s_buffer) - n, "[%s]", thread);
#endif

    n += snprintf(&s_buffer[n], sizeof(s_buffer) - n, "[%s] ", level_str);

    va_list args;
    va_start(args, fmt);
    vsnprintf(&s_buffer[n], sizeof(s_buffer) - n, fmt, args);
    va_end(args);

    strncat(s_buffer, "\r\n",
            sizeof(s_buffer) - strlen(s_buffer) - 1);

    return debug_write(s_buffer);
}

/****************************** End of file *********************************************/

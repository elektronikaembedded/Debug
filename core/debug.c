/**
 * @file      debug.c
 * @brief     Debug and logging module implementation.
 * @version   1.0.0
 * @date      2026-01-02
 * @author    Sarath S
 *
 * @details
 * Implements the debug framework with:
 *  - Log levels: ERROR, WARN, INFO, DEBUG
 *  - Formatted printf-style logging
 *  - Sequence numbers
 *  - Timestamps
 *  - Thread-safe output via abstracted transport and port layers
 *
 * Portable across multiple platforms and vendors using:
 *  - Transport layer (UART, USB CDC, etc.)
 *  - OS/Port abstraction (bare-metal, FreeRTOS)
 *
 * @note
 * Ensure config.h is correctly configured for transport, platform,
 * and log formatting options.
 *
 * @par Contact
 * elektronikaembedded@gmail.com
 *
 * @par Website
 * https://elektronikaembedded.wordpress.com
 */

/** @defgroup DEBUG_MODULE Debug Module
 *  @brief Provides logging and debug utilities.
 *  @{
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "config.h"
#include "debug.h"
#include "debug_transport.h"
#include "debug_port.h"

/*******************************************************************************
 * Private Macros
 *******************************************************************************/

/* Add private macros here if needed */

/*******************************************************************************
 * Private Types
 *******************************************************************************/

/**
 * @brief Debug runtime context structure.
 */
typedef struct
{
    const debug_transport_hal_t *transport;   /**< Active transport HAL */
    const debug_port_t          *debug_port;  /**< OS/platform port */
    log_level_t                  level;       /**< Current log level */
    uint8_t                      initialized; /**< Initialization state */
} debug_context_t;

/*******************************************************************************
 * Private Function Prototypes (Static)
 *******************************************************************************/

/**
 * @brief Generate the next log sequence number (thread-safe via port layer).
 *
 * @return Next sequence number
 */
static uint32_t debug_next_sequence(void);

/*******************************************************************************
 * Private Variables (Static)
 *******************************************************************************/

/** @brief Global debug context */
static debug_context_t debug_ctx = {0};

/** @brief Global log sequence number */
static uint32_t log_sequence_no = 0;

/** @brief Internal buffer for formatted messages */
static char s_buffer[DEBUG_BUFFER_SIZE];

/*******************************************************************************
 * Private Function Definitions (Static)
 *******************************************************************************/

static uint32_t debug_next_sequence(void)
{
    uint32_t seq;

    if ((NULL != debug_ctx.debug_port) &&
        (NULL != debug_ctx.debug_port->ops->lock))
    {
        debug_ctx.debug_port->ops->lock();
    }

    seq = ++log_sequence_no;

    if ((NULL != debug_ctx.debug_port) &&
        (NULL != debug_ctx.debug_port->ops->unlock))
    {
        debug_ctx.debug_port->ops->unlock();
    }

    return seq;
}

/*******************************************************************************
 * Public Function Definitions
 *******************************************************************************/

/**
 * @brief Initialize the debug framework.
 *
 * @param[in] trns_hal   Pointer to transport HAL (UART, USB CDC, etc.)
 * @param[in] debug_port Pointer to OS/platform port abstraction
 *
 * @return 0 on success, negative value on failure
 */
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

    if ((NULL == debug_ctx.transport->ops->init) ||
        (NULL == debug_ctx.debug_port->ops->init))
    {
        return -1;
    }

    if (0 != debug_ctx.transport->ops->init())
    {
        return -8;
    }

    if (0 != debug_ctx.debug_port->ops->init())
    {
        return -8;
    }

    debug_ctx.initialized = 1;
    return 0;
}

/**
 * @brief Set the current log level.
 *
 * @param[in] level Desired log level
 */
void debug_set_level(log_level_t level)
{
    debug_ctx.level = level;
}

/**
 * @brief Get the current log level.
 *
 * @return Current log level
 */
log_level_t debug_get_level(void)
{
    return debug_ctx.level;
}

/**
 * @brief Write a raw string to the debug transport.
 *
 * @param[in] str Null-terminated string
 * @return Number of bytes written, or -1 on error
 */
int debug_write(const char *str)
{
    if (0 == debug_ctx.initialized)
    {
        return 0;
    }

    if ((NULL == debug_ctx.transport) ||
        (NULL == debug_ctx.transport->ops->write) ||
        (NULL == str))
    {
        return -1;
    }

    if (debug_ctx.debug_port && debug_ctx.debug_port->ops->lock)
    {
        debug_ctx.debug_port->ops->lock();
    }

    int ret = debug_ctx.transport->ops->write(
        (const uint8_t *)str, strlen(str));

    if (debug_ctx.debug_port && debug_ctx.debug_port->ops->unlock)
    {
        debug_ctx.debug_port->ops->unlock();
    }

    return ret;
}

/**
 * @brief Print a formatted string to the debug transport.
 *
 * @param[in] fmt Format string (printf-style)
 * @param[in] ... Variable arguments
 * @return Number of bytes written, or -1 on error
 */
int debug_printf(const char *fmt, ...)
{
    if (0 == debug_ctx.initialized)
    {
        return 0;
    }

    if (NULL == fmt)
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

/**
 * @brief Log a formatted message with level filtering and optional metadata.
 *
 * @param[in] level Log level of the message
 * @param[in] fmt   Format string (printf-style)
 * @param[in] ...   Variable arguments
 * @return Number of bytes written, or 0 if filtered
 */
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
    if ((NULL != debug_ctx.debug_port) &&
        (NULL != debug_ctx.debug_port->ops->get_timestamp))
    {
        ts = debug_ctx.debug_port->ops->get_timestamp();
    }
#endif

#if DEBUG_ENABLE_THREAD_INFO == YES
    if ((NULL != debug_ctx.debug_port) &&
        (NULL != debug_ctx.debug_port->ops->get_thread_name))
    {
        thread = debug_ctx.debug_port->ops->get_thread_name();
    }
#endif

#if DEBUG_ENABLE_SEQUENCE_NO == YES
    seq = debug_next_sequence();
#endif

    const char *level_str = "LOG";
    if (level == LOG_ERROR) level_str = "ERROR";
    else if (level == LOG_WARN)  level_str = "WARN";
    else if (level == LOG_INFO)  level_str = "INFO";
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

/** @} */ // End of DEBUG_MODULE

/*******************************************************************************
 * End of file
 *******************************************************************************/

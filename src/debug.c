/*******************************************************************************
 * @file      debug.c
 * @brief     Debug and logging module implementation
 *
 * @details
 * Provides formatted logging with:
 *  - Log levels
 *  - Optional timestamp / sequence / thread info
 *  - Thread-safe output using OSAL (if available)
 *
 *******************************************************************************/


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include "debug.h"
#include "debug_transport.h"

#ifdef DEBUG_CONFIG_H
#include "debug_config.h"
#else
#error "debug_config.h not found or not included!, \
please use the debug/template/debug_config.h.example"
#endif

#ifdef OSAL_H
#include "osal.h"
#else
#error "osal.h not found or not included!, \
please pull the osal module https://github.com/elektronikaembedded/osal"
#endif


//#include "debug_config.h"
/*******************************************************************************
 * Private Types
 *******************************************************************************/

/**
 * @brief Internal debug context (singleton)
 */
typedef struct
{
    const debug_transport_t *transport; /**< Output transport */
#ifdef DEBUG_USE_OSAL
    const osal_t *osal;                     /**< OS abstraction */
#endif
    log_level_t level;                      /**< Active log level */
    uint8_t initialized;                    /**< Init flag */
} debug_context_t;

/*******************************************************************************
 * Private Variables
 *******************************************************************************/

static debug_context_t debug_ctx = {0};   /**< Global context */
static uint32_t log_sequence_no = 0;      /**< Sequence counter */
static char s_buffer[DEBUG_BUFFER_SIZE];  /**< Internal buffer */

/*******************************************************************************
 * Private Functions
 *******************************************************************************/

/**
 * @brief Get next sequence number (thread-safe if OSAL used)
 */
static uint32_t debug_next_sequence(void)
{
    uint32_t seq;

#ifdef DEBUG_USE_OSAL
    if (debug_ctx.osal && debug_ctx.osal->ops->lock)
    {
        debug_ctx.osal->ops->lock();
    }
#endif

    seq = ++log_sequence_no;

#ifdef DEBUG_USE_OSAL
    if (debug_ctx.osal && debug_ctx.osal->ops->unlock)
    {
        debug_ctx.osal->ops->unlock();
    }
#endif

    return seq;
}

/*******************************************************************************
 * Public Functions
 *******************************************************************************/

int debug_init(const debug_transport_t *trns,
#ifdef DEBUG_USE_OSAL
               const osal_t *osal
#else
               void *unused
#endif
)
{
    if (NULL == trns)
    {
    	return -1; /* error null pointer */
    }

    debug_ctx.transport = trns;
    debug_ctx.level = LOG_DEBUG;
    debug_ctx.initialized = 0;

#ifdef DEBUG_USE_OSAL
    if (NULL == osal)
    {
    	return -1; /* error null pointer */
    }
    debug_ctx.osal = osal;
#endif

    /* Validate required transport */
    if ((NULL == debug_ctx.transport->ops) ||
        (NULL == debug_ctx.transport->ops->init) ||
        (NULL == debug_ctx.transport->ops->write))
    {
    	return -1; /* error null pointer */
    }

    /* Init transport */
    if (0 != debug_ctx.transport->ops->init())
    {
    	return -1; /* error null pointer */
    }

#ifdef DEBUG_USE_OSAL
    /* Init OSAL if provided */
    if (debug_ctx.osal &&
        debug_ctx.osal->ops &&
        debug_ctx.osal->ops->init)
    {
        debug_ctx.osal->ops->init(debug_ctx.osal);
    }
#endif

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

/**
 * @brief Write raw string
 */
int debug_write(const char *str)
{
    if ((0 == debug_ctx.initialized) || (NULL == str))
    {
        return 0;
    }

    if ((NULL == debug_ctx.transport) ||
        (NULL == debug_ctx.transport->ops->write))
    {
    	return -1; /* error null pointer */
    }

#ifdef DEBUG_USE_OSAL
    if (debug_ctx.osal && debug_ctx.osal->ops->lock)
    {
        debug_ctx.osal->ops->lock();
    }
#endif

    int ret = debug_ctx.transport->ops->write(
        (const uint8_t *)str, strlen(str));

#ifdef DEBUG_USE_OSAL
    if (debug_ctx.osal && debug_ctx.osal->ops->unlock)
    {
        debug_ctx.osal->ops->unlock();
    }
#endif

    return ret;
}

/**
 * @brief Print formatted string
 */
int debug_printf(const char *fmt, ...)
{
    if ((0 == debug_ctx.initialized) || (NULL == fmt))
    {
        return 0;
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
 * @brief Log message with level filtering
 */
int debug_log(log_level_t level, const char *fmt, ...)
{
    if ((0 == debug_ctx.initialized) || (level > debug_ctx.level))
    {
        return 0; /* filtered */
    }

    uint32_t ts = 0;
    uint32_t seq = 0;
    const char *thread = "MAIN";

#ifdef DEBUG_USE_OSAL
#if DEBUG_ENABLE_TIME_DATE_INFO == YES
    if (debug_ctx.osal && debug_ctx.osal->ops->get_timestamp)
    {
        ts = debug_ctx.osal->ops->get_timestamp();
    }
#endif

#if DEBUG_ENABLE_THREAD_INFO == YES
    if (debug_ctx.osal && debug_ctx.osal->ops->get_thread_name)
    {
        thread = debug_ctx.osal->ops->get_thread_name();
    }
#endif
#endif

#if DEBUG_ENABLE_SEQUENCE_NO == YES
    seq = debug_next_sequence();
#endif

    /* Level string */
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

/*******************************************************************************
 * End of file
 *******************************************************************************/

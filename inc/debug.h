/******************************************************************************
 * @file    debug.h
 * @brief   Lightweight debug/logging interface
 *
 * @details
 * Simple and portable logging module for embedded systems.
 *
 * Features:
 *  - Log levels (ERROR, WARN, INFO, DEBUG)
 *  - printf-style logging
 *  - Optional timestamp, sequence, thread info
 *  - Transport abstraction (UART, USB, etc.)
 *  - OS abstraction support (optional)
 *
 * Designed to work with:
 *  - Bare-metal
 *  - FreeRTOS / Zephyr (via OSAL)
 *
 ******************************************************************************/

#ifndef DEBUG_H
#define DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 *******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "debug_transport.h"
#include "osal.h"
#include "debug_config.h"
/*******************************************************************************
 * Types
 *******************************************************************************/
/**
 * @enum log_level_t
 * @brief Defines the severity of debug log messages.
 *
 * Messages below the configured log level are filtered out.
 */
typedef enum
{
    LOG_ERROR = 0,   /*!< Critical error conditions */
    LOG_WARN,        /*!< Warning messages */
    LOG_INFO,        /*!< Informational messages */
    LOG_DEBUG        /*!< Detailed debug messages for troubleshooting */
} log_level_t;

/**
 * @brief Debug return codes
 */
typedef enum
{
    DEBUG_OK = 0,
    DEBUG_ERR_NULL_PTR = -1,
    DEBUG_ERR_INVALID_ARGS = -2,
    DEBUG_ERR_NOT_IMPD = -3
} debug_err_t;

/*******************************************************************************
 * Macros
 *******************************************************************************/

/* Logging macros: compile to no-ops if DEBUG_ENABLE == NO */
#if DEBUG_ENABLE == YES

/** @brief Log an error-level message. */
#define LOG_ERROR(...)  debug_log(LOG_ERROR, __VA_ARGS__)

/** @brief Log a warning-level message. */
#define LOG_WARN(...)   debug_log(LOG_WARN,  __VA_ARGS__)

/** @brief Log an informational message. */
#define LOG_INFO(...)   debug_log(LOG_INFO,  __VA_ARGS__)

/** @brief Log a debug-level message. */
#define LOG_DEBUG(...)  debug_log(LOG_DEBUG, __VA_ARGS__)

#else  /* DEBUG_ENABLE == NO */

#define LOG_ERROR(...)
#define LOG_WARN(...)
#define LOG_INFO(...)
#define LOG_DEBUG(...)

#endif /* DEBUG_ENABLE */

/*******************************************************************************
 * API
 *******************************************************************************/
/**
 * @brief Initialize the debug framework.
 *
 * @param[in] trns_hal   Pointer to the active debug transport HAL
 * @param[in] debug_port Pointer to the debug port abstraction
 *
 * @retval 0   Initialization succeeded
 * @retval -1  Initialization failed
 *
 * @note The transport HAL must provide a valid write() function.
 */
#ifdef DEBUG_USE_OSAL
int debug_init(const debug_transport_t *trns,
                       const osal_t *osal);
#else
int debug_init(const debug_transport_t *trns,
                       void *unused);
#endif

/**
 * @brief Set the current debug log level.
 *
 * @param[in] level Desired log verbosity
 */
void debug_set_level(log_level_t level);

/**
 * @brief Get the current debug log level.
 *
 * @return Currently configured log level
 */
log_level_t debug_get_level(void);

/**
 * @brief Write a raw string to the debug output.
 *
 * @param[in] str Null-terminated string to write
 *
 * @retval >=0  Number of bytes successfully written
 * @retval -1   Error occurred
 */
int debug_write(const char *str);

/**
 * @brief Print a formatted string to the debug output.
 *
 * @param[in] fmt printf-style format string
 * @param[in] ... Variable arguments
 *
 * @retval >=0  Number of bytes successfully written
 * @retval -1   Error occurred
 */
int debug_printf(const char *fmt, ...);

/**
 * @brief Log a formatted message with severity and metadata.
 *
 * @param[in] level Log severity level
 * @param[in] fmt   printf-style format string
 * @param[in] ...   Variable arguments
 *
 * @retval >=0  Number of bytes successfully written
 * @retval 0    Message filtered by current log level
 * @retval -1   Error occurred
 */
int debug_log(log_level_t level, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* DEBUG_H */

/****************************** End of file ***********************************/

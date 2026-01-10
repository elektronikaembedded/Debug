/**
 * @file      debug.h
 * @brief     Public interface for the debug and logging framework.
 * @version   1.0.0
 * @date      2025-08-07
 * @author    Sarath S
 *
 * @details
 * Provides the API for the debug framework, including:
 *  - Log levels: ERROR, WARN, INFO, DEBUG
 *  - Formatted printf-style logging
 *  - Optional sequence numbers and timestamps
 *  - Thread-safe output via abstracted transport and port layers
 *
 * Portable across bare-metal and RTOS-based systems through platform
 * abstraction layers.
 *
 * @par Contact
 * elektronikaembedded@gmail.com
 *
 * @par Website
 * https://elektronikaembedded.wordpress.com
 */

/** @defgroup DEBUG_MODULE Debug Module
 *  @brief Public interface for the debug/logging framework.
 *  @{
 */

#ifndef DEBUG_H
#define DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 *******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#include "config.h"
#include "debug_transport.h"
#include "debug_port.h"

/*******************************************************************************
 * Private Macros
 *******************************************************************************/

/*******************************************************************************
 * Private Types
 *******************************************************************************/

/*******************************************************************************
 * Private Variables (Static)
 *******************************************************************************/

/*******************************************************************************
 * Private Function Prototypes (Static)
 *******************************************************************************/

/*******************************************************************************
 * Public Types
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
 * Public Function Declarations
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
int debug_init(const debug_transport_hal_t *trns_hal,
               const debug_port_t *debug_port);

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

/** @} */ // End of DEBUG_MODULE

/*******************************************************************************
 * End of file
 *******************************************************************************/

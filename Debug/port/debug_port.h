/**
 * @file      debug_port.h
 * @brief     Debug port abstraction layer for the debug framework.
 * @version   1.0.0
 * @date      2026-01-02
 * @author    Sarath S
 *
 * @details
 * Provides a platform/OS abstraction layer used by the debug framework.
 * Supports services like:
 *  - Locking / thread-safe output
 *  - ISR detection
 *  - Timestamp retrieval
 *  - Thread/task name retrieval
 *
 * The actual port implementation (FreeRTOS or Bare-metal) is selected at
 * compile time via macros in config.h.
 *
 * @par Contact
 * elektronikaembedded@gmail.com
 *
 * @par Website
 * https://elektronikaembedded.wordpress.com
 */

/** @defgroup DEBUG_PORT Debug Port Layer
 *  @brief Abstraction layer for platform-specific debug services.
 *  @{
 */

#ifndef DEBUG_PORT_H
#define DEBUG_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 *******************************************************************************/
#include <stdint.h>
#include "config.h"
#include "common.h"

/*******************************************************************************
 * Private Macros
 *******************************************************************************/
/* Add private macros here if needed */

/*******************************************************************************
 * Private Types
 *******************************************************************************/
/* Add private types or enums here if needed */

/*******************************************************************************
 * Private Variables (Static)
 *******************************************************************************/
/* Add static/private variables here if needed */

/*******************************************************************************
 * Private Function Prototypes (Static)
 *******************************************************************************/
/* Add static/private function prototypes here if needed */

/*******************************************************************************
 * Public Types
 *******************************************************************************/

/**
 * @brief Debug port operations structure
 *
 * Defines platform- or OS-specific callbacks used by the debug framework
 * for synchronization, execution context detection, and system info retrieval.
 */
typedef struct
{
    int  (*init)(void);            /**< Initialize debug port */
    int  (*deinit)(void);          /**< Deinitialize debug port */
    void (*lock)(void);            /**< Lock debug output (thread-safe) */
    void (*unlock)(void);          /**< Unlock debug output */
    uint32_t (*get_timestamp)(void);      /**< Retrieve system timestamp */
    int  (*is_isr)(void);          /**< Check if currently in ISR context */
    const char *(*get_thread_name)(void); /**< Get current thread/task name */
} debug_port_ops_t;

/**
 * @brief Debug port descriptor
 *
 * Holds reference to the selected platform-specific debug port operations table.
 */
typedef struct
{
    const debug_port_ops_t *ops; /**< Debug port operations table */
} debug_port_t;

/*******************************************************************************
 * Public Function Declarations
 *******************************************************************************/

/**
 * @brief Initialize debug port
 *
 * Selects and initializes the appropriate debug port implementation
 * based on compile-time configuration macros in config.h.
 *
 * @param[in,out] port Pointer to debug_port_t instance
 *
 * @retval 0   Port initialized successfully
 * @retval -1  Invalid pointer or initialization failure
 */
int debug_port_init(debug_port_t *port);

/**
 * @brief Deinitialize debug port
 *
 * Calls the selected debug port's deinitialization function if available.
 *
 * @param[in,out] port Pointer to debug_port_t instance
 *
 * @retval 0   Port deinitialized successfully
 * @retval -1  Invalid pointer or deinitialization failure
 */
int debug_port_deinit(debug_port_t *port);

#ifdef __cplusplus
}
#endif

#endif /* DEBUG_PORT_H */

/** @} */ // End of DEBUG_PORT

/*******************************************************************************
 * End of file
 *******************************************************************************/

/****************************************************************************************
 * @file        debug_port.h
 * @author      Sarath S
 * @date        2025-08-07
 * @version     1.0
 * @brief       Debug port abstraction layer
 *
 * @details
 * This file defines the platform/OS abstraction layer used by the debug framework.
 * The debug port provides services such as locking, ISR detection, timestamp retrieval,
 * and thread name access, allowing the debug core to remain OS-agnostic.
 *
 * @contact     elektronikaembedded@gmail.com
 * @website     https://elektronikaembedded.wordpress.com
 ****************************************************************************************/

#ifndef DEBUG_PORT_H
#define DEBUG_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************** Header include files ************************************/
#include "config.h"
#include "common.h"

/****************************** Type definitions ****************************************/

/**
 * @brief Debug port operations structure
 *
 * Defines platform- or OS-specific callbacks used by the debug framework
 * for synchronization and execution context information.
 */
typedef struct
{
    /**
     * @brief Initialize debug port
     * @return 0 on success, -1 on error
     */
    int (*init)(void);

    /**
     * @brief Deinitialize debug port
     * @return 0 on success, -1 on error
     */
    int (*deinit)(void);

    /**
     * @brief Lock debug output (thread-safe)
     */
    void (*lock)(void);

    /**
     * @brief Unlock debug output
     */
    void (*unlock)(void);

    /**
     * @brief Get system timestamp
     * @return Timestamp value (platform-defined units)
     */
    uint32_t (*get_timestamp)(void);

    /**
     * @brief Check whether current context is ISR
     * @return 1 if in ISR context, 0 otherwise
     */
    int (*is_isr)(void);

    /**
     * @brief Get current thread or task name
     * @return Pointer to null-terminated thread name string
     */
    const char *(*get_thread_name)(void);

} debug_port_ops_t;

/**
 * @brief Debug port descriptor
 *
 * Holds the platform-specific debug port operations table.
 */
typedef struct debug_port
{
    debug_port_ops_t debug_port_ops; /**< Debug port operations */
} debug_port_t;

/****************************** Function declarations ************************************/

/**
 * @brief           Initialize debug port
 * @param[in]       port   Pointer to debug_port_t instance
 * @return          0 on success, -1 on error
 *
 * @note
 * The actual port implementation is selected based on config.h.
 */
int debug_port_init(debug_port_t *port);

#ifdef __cplusplus
}
#endif

#endif /* DEBUG_PORT_H */

/****************************** End of this file ****************************************/

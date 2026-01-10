/****************************************************************************************
 * @file        debug_port_baremetal.c
 * @author      Sarath S
 * @date        2026-01-02
 * @version     1.1
 * @brief       Bare-metal debug port implementation
 *
 * @details
 * Implements the bare-metal debug port layer.
 * Provides minimal OS abstraction services for the debug framework:
 *   - Locking / unlocking (no-op)
 *   - ISR detection (CMSIS-based)
 *   - Timestamp retrieval (stub, user-overridable)
 *   - Thread name access (returns "MAIN" or "ISR")
 *
 * @contact     elektronikaembedded@gmail.com
 * @website     https://elektronikaembedded.wordpress.com
 ****************************************************************************************/

#include "config.h"

#if DEBUG_USE_BAREMETAL

/****************************** Header include files ************************************/
#include <stdint.h>
#include "debug_port_baremetal.h"
#include "debug_port.h"

/* CMSIS core header for __get_IPSR() */
#if defined(__ARM_ARCH)
#include "cmsis_gcc.h"
#endif

/****************************** Static function prototypes ******************************/
static int      debug_port_baremetal_init(void);
static int      debug_port_baremetal_deinit(void);
static void     debug_port_baremetal_lock(void);
static void     debug_port_baremetal_unlock(void);
static uint32_t debug_port_baremetal_get_timestamp(void);
static int      debug_port_baremetal_is_isr(void);
static const char *debug_port_baremetal_get_thread_name(void);

/****************************** Static variable definitions ******************************/
static const debug_port_ops_t DEBUG_PORT_BAREMETAL_OPS =
{
    .init            = debug_port_baremetal_init,
    .deinit          = debug_port_baremetal_deinit,
    .lock            = debug_port_baremetal_lock,
    .unlock          = debug_port_baremetal_unlock,
    .get_timestamp   = debug_port_baremetal_get_timestamp,
    .is_isr          = debug_port_baremetal_is_isr,
    .get_thread_name = debug_port_baremetal_get_thread_name
};

/****************************** Function definitions ************************************/

/**
 * @brief Initialize bare-metal debug port
 *
 * @return 0 on success
 */
static int debug_port_baremetal_init(void)
{
    return 0;
}

/**
 * @brief Deinitialize bare-metal debug port
 *
 * @return 0 on success
 */
static int debug_port_baremetal_deinit(void)
{
    return 0;
}

/**
 * @brief Lock debug output (no-op)
 */
static void debug_port_baremetal_lock(void)
{
    /* No RTOS, no locking required */
}

/**
 * @brief Unlock debug output (no-op)
 */
static void debug_port_baremetal_unlock(void)
{
    /* No RTOS, no unlocking required */
}

/**
 * @brief Get system timestamp
 *
 * @return Timestamp value (stub: returns 0)
 *
 * @note
 * User may override this function to return:
 *   - SysTick
 *   - DWT cycle counter
 *   - Hardware timer
 */
static uint32_t debug_port_baremetal_get_timestamp(void)
{
    return 0U;
}

/**
 * @brief Check if current context is ISR
 *
 * @return 1 if ISR context, 0 otherwise
 */
static int debug_port_baremetal_is_isr(void)
{
#if defined(__ARM_ARCH)
    return (__get_IPSR() != 0U);
#else
    return 0;
#endif
}

/**
 * @brief Get current execution context name
 *
 * @return "ISR" or "MAIN"
 */
static const char *debug_port_baremetal_get_thread_name(void)
{
    return debug_port_baremetal_is_isr() ? "ISR" : "MAIN";
}

/**
 * @brief Get bare-metal debug port operations table
 *
 * @return Pointer to operations table
 */
const debug_port_ops_t *debug_port_baremetal_ops(void)
{
    return &DEBUG_PORT_BAREMETAL_OPS;
}

#endif /* DEBUG_USE_BAREMETAL */

/****************************** End of file *********************************************/

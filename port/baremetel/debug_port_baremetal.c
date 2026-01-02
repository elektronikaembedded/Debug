/****************************************************************************************
 * @file        debug_port_baremetal.c
 * @author      Sarath S
 * @date        2026-01-02
 * @version     1.0
 * @brief       Bare-metal debug port implementation
 *
 * @details
 * Implements the bare-metal debug port layer.
 * Provides minimal OS abstraction services for the debug framework:
 *   - Locking / unlocking (no-op)
 *   - ISR detection (processor-specific or stub)
 *   - Timestamp retrieval (stub)
 *   - Thread name access (always "MAIN" or "ISR")
 *
 * @contact     elektronikaembedded@gmail.com
 * @website     https://elektronikaembedded.wordpress.com
 ****************************************************************************************/

#include "config.h"

#if DEBUG_USE_BAREMETAL
#include <stdint.h>
#include "debug_port_baremetal.h"
#include "debug_port.h"
#if DEBUG_VENDOR_STM32
#include "stm32f4xx.h"

#elif DEBUG_VENDOR_NXP
#include "nxpxxxxxx.h"  /* Replace with actual NXP device header (e.g., MK64F12.h) */

#elif DEBUG_VENDOR_TI
#include "tixxxxxx.h"

#else
#error "No vendor selected for bare-metal port"
#endif


/****************************** Static function prototypes ******************************/
static int debug_port_baremetal_init(void);
static int debug_port_baremetal_deinit(void);
static void debug_port_baremetal_lock(void);
static void debug_port_baremetal_unlock(void);
static uint32_t debug_port_baremetal_get_timestamp(void);
static int debug_port_baremetal_is_isr(void);
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

/*!----------------------------------------------------------------------------
 * \brief           Initialize bare-metal debug port
 * \param[in]       None
 * \param[out]      None
 * \param[in/out]   None
 * \return          0 on success
 * \note            Minimal setup, placeholder for future use
 *---------------------------------------------------------------------------*/
static int debug_port_baremetal_init(void)
{
    return 0;
}

/*!----------------------------------------------------------------------------
 * \brief           Deinitialize bare-metal debug port
 * \param[in]       None
 * \param[out]      None
 * \param[in/out]   None
 * \return          0 on success
 * \note            Placeholder for future use
 *---------------------------------------------------------------------------*/
static int debug_port_baremetal_deinit(void)
{
    return 0;
}

/*!----------------------------------------------------------------------------
 * \brief           Lock debug output (no-op)
 * \param[in]       None
 * \param[out]      None
 * \param[in/out]   None
 * \return          None
 * \note            No OS, so locking is not required
 *---------------------------------------------------------------------------*/
static void debug_port_baremetal_lock(void)
{
    /* No operation */
}

/*!----------------------------------------------------------------------------
 * \brief           Unlock debug output (no-op)
 * \param[in]       None
 * \param[out]      None
 * \param[in/out]   None
 * \return          None
 * \note            No OS, so unlocking is not required
 *---------------------------------------------------------------------------*/
static void debug_port_baremetal_unlock(void)
{
    /* No operation */
}

/*!----------------------------------------------------------------------------
 * \brief           Get system timestamp
 * \param[in]       None
 * \param[out]      None
 * \param[in/out]   None
 * \return          0 (stub)
 * \note            No system tick available on bare-metal
 *---------------------------------------------------------------------------*/
static uint32_t debug_port_baremetal_get_timestamp(void)
{
    return 0;
}

/*!----------------------------------------------------------------------------
 * \brief           Check if current context is ISR
 * \param[in]       None
 * \param[out]      None
 * \param[in/out]   None
 * \return          1 if in ISR, 0 otherwise
 * \note            Uses processor interrupt status register
 *---------------------------------------------------------------------------*/
static int debug_port_baremetal_is_isr(void)
{
    return (__get_IPSR() != 0);
}

/*!----------------------------------------------------------------------------
 * \brief           Get current thread name
 * \param[in]       None
 * \param[out]      None
 * \param[in/out]   None
 * \return          "ISR" if in interrupt, otherwise "MAIN"
 * \note            No RTOS threads, so only main or ISR
 *---------------------------------------------------------------------------*/
static const char *debug_port_baremetal_get_thread_name(void)
{
    if (__get_IPSR() != 0)
    {
        return "ISR";
    }
    return "MAIN";
}

/*!----------------------------------------------------------------------------
 * \brief           Get bare-metal debug port operations table
 * \param[in]       None
 * \param[out]      None
 * \param[in/out]   None
 * \return          Pointer to bare-metal debug port operations table
 * \note            Used by debug_port_init for bare-metal systems
 *---------------------------------------------------------------------------*/
const debug_port_ops_t *debug_port_baremetal_ops(void)
{
    return &DEBUG_PORT_BAREMETAL_OPS;
}

#endif /* DEBUG_USE_BAREMETAL */

/****************************** End of file *********************************************/

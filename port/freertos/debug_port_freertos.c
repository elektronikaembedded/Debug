/****************************************************************************************
 * @file        debug_port_freertos.c
 * @author      Sarath S
 * @date        2025-08-15
 * @version     1.0
 * @brief       FreeRTOS debug port implementation
 *
 * @details
 * Implements the FreeRTOS-specific debug port layer.
 * Provides OS abstraction services such as locking, ISR detection,
 * timestamp retrieval, and thread name access for the debug framework.
 *
 *
 * @contact     elektronikaembedded@gmail.com
 * @website     https://elektronikaembedded.wordpress.com
 ****************************************************************************************/

#include "config.h"

#if DEBUG_USE_FREERTOS

#include "debug_port_freertos.h"
#include "debug_port.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "core_cm4.h"  /* Replace with correct core header for __get_IPSR */

/****************************** Static function prototypes ******************************/
static int debug_port_freertos_init(void);
static int debug_port_freertos_deinit(void);
static void debug_port_freertos_lock(void);
static void debug_port_freertos_unlock(void);
static uint32_t debug_port_freertos_get_timestamp(void);
static int debug_port_freertos_is_isr(void);
static const char *debug_port_freertos_get_thread_name(void);

/****************************** Static variable definitions ******************************/
static SemaphoreHandle_t debug_mutex;

/*!
 * \brief FreeRTOS debug port operations table
 */
static const debug_port_ops_t DEBUG_PORT_FREERTOS_OPS =
{
    .init             = debug_port_freertos_init,
    .deinit           = debug_port_freertos_deinit,
    .lock             = debug_port_freertos_lock,
    .unlock           = debug_port_freertos_unlock,
    .get_timestamp    = debug_port_freertos_get_timestamp,
    .is_isr           = debug_port_freertos_is_isr,
    .get_thread_name  = debug_port_freertos_get_thread_name
};

/****************************** Function definitions ************************************/

/*!----------------------------------------------------------------------------
 * \brief           Initialize FreeRTOS debug port
 * \param[in]       None
 * \param[out]      None
 * \param[in/out]   None
 * \return          0 on success
 * \note            Creates a mutex for thread-safe debug output
 *---------------------------------------------------------------------------*/
static int debug_port_freertos_init(void)
{
    debug_mutex = xSemaphoreCreateMutex();
    xSemaphoreGive(debug_mutex);
    return 0;
}

/*!----------------------------------------------------------------------------
 * \brief           Deinitialize FreeRTOS debug port
 * \param[in]       None
 * \param[out]      None
 * \param[in/out]   None
 * \return          0 on success
 * \note            Deletes the mutex; reserved for future use
 *---------------------------------------------------------------------------*/
static int debug_port_freertos_deinit(void)
{
    vSemaphoreDelete(debug_mutex);
    return 0;
}

/*!----------------------------------------------------------------------------
 * \brief           Lock debug output (thread-safe)
 * \param[in]       None
 * \param[out]      None
 * \param[in/out]   None
 * \return          None
 * \note            Do not call from ISR context
 *---------------------------------------------------------------------------*/
static void debug_port_freertos_lock(void)
{
    if (!debug_port_freertos_is_isr())
    {
        xSemaphoreTake(debug_mutex, portMAX_DELAY);
    }
}

/*!----------------------------------------------------------------------------
 * \brief           Unlock debug output
 * \param[in]       None
 * \param[out]      None
 * \param[in/out]   None
 * \return          None
 * \note            Do not call from ISR context
 *---------------------------------------------------------------------------*/
static void debug_port_freertos_unlock(void)
{
    if (!debug_port_freertos_is_isr())
    {
        xSemaphoreGive(debug_mutex);
    }
}

/*!----------------------------------------------------------------------------
 * \brief           Get system timestamp
 * \param[in]       None
 * \param[out]      None
 * \param[in/out]   None
 * \return          System tick count
 * \note            Uses FreeRTOS tick counter
 *---------------------------------------------------------------------------*/
static uint32_t debug_port_freertos_get_timestamp(void)
{
    return xTaskGetTickCount();
}

/*!----------------------------------------------------------------------------
 * \brief           Check if current context is ISR
 * \param[in]       None
 * \param[out]      None
 * \param[in/out]   None
 * \return          1 if in ISR, 0 otherwise
 * \note            Uses processor interrupt status register
 *---------------------------------------------------------------------------*/
static int debug_port_freertos_is_isr(void)
{
    return (__get_IPSR() != 0);
}

/*!----------------------------------------------------------------------------
 * \brief           Get current thread name
 * \param[in]       None
 * \param[out]      None
 * \param[in/out]   None
 * \return          Pointer to thread name string
 * \note            Returns "ISR" when called from interrupt context
 *---------------------------------------------------------------------------*/
static const char *debug_port_freertos_get_thread_name(void)
{
    if (__get_IPSR() != 0)
    {
        return "ISR";
    }

    TaskHandle_t h = xTaskGetCurrentTaskHandle();
    return pcTaskGetName(h);
}

/*!----------------------------------------------------------------------------
 * \brief           Get FreeRTOS debug port operations table
 * \param[in]       None
 * \param[out]      None
 * \param[in/out]   None
 * \return          Pointer to FreeRTOS debug port operations table
 * \note            Used by debug_port_init for FreeRTOS systems
 *---------------------------------------------------------------------------*/
const debug_port_ops_t *debug_port_freertos_ops(void)
{
    return &DEBUG_PORT_FREERTOS_OPS;
}

#endif /* DEBUG_USE_FREERTOS */

/****************************** End of file *********************************************/

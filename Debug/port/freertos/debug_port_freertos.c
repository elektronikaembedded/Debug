/****************************************************************************************
 * @file        debug_port_freertos.c
 * @author      Sarath S
 * @date        2025-08-15
 * @version     1.1
 * @brief       FreeRTOS debug port implementation
 *
 * @details
 * Implements the FreeRTOS-specific debug port layer.
 * Provides OS abstraction services such as locking, ISR detection,
 * timestamp retrieval, and thread name access for the debug framework.
 *
 * @contact     elektronikaembedded@gmail.com
 * @website     https://elektronikaembedded.wordpress.com
 ****************************************************************************************/

#include "config.h"

#if DEBUG_USE_FREERTOS

/****************************** Header include files ************************************/
#include "debug_port_freertos.h"
#include "debug_port.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "core_cm4.h"  /* Replace with correct core header if needed */

/****************************** Static function prototypes ******************************/
static int      debug_port_freertos_init(void);
static int      debug_port_freertos_deinit(void);
static void     debug_port_freertos_lock(void);
static void     debug_port_freertos_unlock(void);
static uint32_t debug_port_freertos_get_timestamp(void);
static int      debug_port_freertos_is_isr(void);
static const char *debug_port_freertos_get_thread_name(void);

/****************************** Static variables ****************************************/
static SemaphoreHandle_t debug_mutex = NULL;

/**
 * @brief FreeRTOS debug port operations table
 */
static const debug_port_ops_t DEBUG_PORT_FREERTOS_OPS =
{
    .init            = debug_port_freertos_init,
    .deinit          = debug_port_freertos_deinit,
    .lock            = debug_port_freertos_lock,
    .unlock          = debug_port_freertos_unlock,
    .get_timestamp   = debug_port_freertos_get_timestamp,
    .is_isr          = debug_port_freertos_is_isr,
    .get_thread_name = debug_port_freertos_get_thread_name
};

/****************************** Function definitions ************************************/

/**
 * @brief Initialize FreeRTOS debug port
 *
 * @return 0 on success, -1 on failure
 *
 * @note Creates a mutex for thread-safe debug output.
 */
static int debug_port_freertos_init(void)
{
    debug_mutex = xSemaphoreCreateMutex();
    return (debug_mutex != NULL) ? 0 : -1;
}

/**
 * @brief Deinitialize FreeRTOS debug port
 *
 * @return 0 on success
 */
static int debug_port_freertos_deinit(void)
{
    if (debug_mutex != NULL)
    {
        vSemaphoreDelete(debug_mutex);
        debug_mutex = NULL;
    }
    return 0;
}

/**
 * @brief Lock debug output
 *
 * @note No-op when called from ISR context
 */
static void debug_port_freertos_lock(void)
{
    if ((debug_mutex != NULL) && !debug_port_freertos_is_isr())
    {
        (void)xSemaphoreTake(debug_mutex, portMAX_DELAY);
    }
}

/**
 * @brief Unlock debug output
 *
 * @note No-op when called from ISR context
 */
static void debug_port_freertos_unlock(void)
{
    if ((debug_mutex != NULL) && !debug_port_freertos_is_isr())
    {
        (void)xSemaphoreGive(debug_mutex);
    }
}

/**
 * @brief Get system timestamp
 *
 * @return Tick count
 *
 * @note ISR-safe
 */
static uint32_t debug_port_freertos_get_timestamp(void)
{
    return debug_port_freertos_is_isr() ?
           xTaskGetTickCountFromISR() :
           xTaskGetTickCount();
}

/**
 * @brief Check whether current context is ISR
 *
 * @return 1 if ISR context, 0 otherwise
 */
static int debug_port_freertos_is_isr(void)
{
    return (__get_IPSR() != 0U);
}

/**
 * @brief Get current thread name
 *
 * @return Thread name or static identifier
 */
static const char *debug_port_freertos_get_thread_name(void)
{
    if (debug_port_freertos_is_isr())
    {
        return "ISR";
    }

    TaskHandle_t task = xTaskGetCurrentTaskHandle();
    const char *name = pcTaskGetName(task);

    return (name != NULL) ? name : "TASK";
}

/**
 * @brief Get FreeRTOS debug port operations table
 *
 * @return Pointer to operations table
 */
const debug_port_ops_t *debug_port_freertos_ops(void)
{
    return &DEBUG_PORT_FREERTOS_OPS;
}

#endif /* DEBUG_USE_FREERTOS */

/****************************** End of file *********************************************/

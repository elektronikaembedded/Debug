/**
 * @file      debug_port.c
 * @brief     Debug port selection and initialization.
 * @version   1.0.0
 * @date      2026-01-02
 * @author    Sarath S
 *
 * @details
 * This module selects and initializes the appropriate debug port
 * implementation based on compile-time options defined in config.h.
 *
 * Supported port layers:
 *  - FreeRTOS
 *  - Bare-metal
 *
 * Provides a unified interface for synchronization, timing,
 * and thread-related services to keep the debug framework
 * OS-agnostic.
 *
 * @par Contact
 * elektronikaembedded@gmail.com
 *
 * @par Website
 * https://elektronikaembedded.wordpress.com
 */

/** @defgroup DEBUG_PORT Debug Port Layer
 *  @brief Debug port abstraction and initialization.
 *  @{
 */

#include "config.h"
#include "debug_port.h"

#if DEBUG_USE_FREERTOS
#include "debug_port_freertos.h"
#endif

#if DEBUG_USE_BAREMETAL
#include "debug_port_baremetal.h"
#endif

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
 * Private Function Definitions (Static)
 *******************************************************************************/

/*******************************************************************************
 * Public Function Definitions
 *******************************************************************************/

/**
 * @brief Initialize the debug port layer.
 *
 * @param[in,out] port Pointer to a debug port abstraction instance.
 *
 * @retval 0   Port initialized successfully
 * @retval -1  Invalid port pointer or initialization failure
 *
 * @note
 * The port backend (FreeRTOS or Bare-metal) is selected at compile time
 * via config.h options.
 */
int debug_port_init(debug_port_t *port)
{
    if (NULL == port)
    {
        return -1;
    }

#if DEBUG_USE_FREERTOS
    port->ops = debug_port_freertos_ops();
#elif DEBUG_USE_BAREMETAL
    port->ops = debug_port_baremetal_ops();
#else
#error "No debug port selected! Define DEBUG_USE_FREERTOS or DEBUG_USE_BAREMETAL in config.h"
#endif

    if (NULL != port->ops->init)
    {
        return port->ops->init();
    }

    return 0;
}

/**
 * @brief Deinitialize the debug port layer.
 *
 * @param[in,out] port Pointer to a debug port abstraction instance.
 *
 * @retval 0   Port deinitialized successfully
 * @retval -1  Invalid port pointer or deinitialization failure
 *
 * @note
 * Calls the selected port's deinit() function if implemented.
 */
int debug_port_deinit(debug_port_t *port)
{
    if (NULL == port)
    {
        return -1;
    }

    if (NULL != port->ops->deinit)
    {
        return port->ops->deinit();
    }

    return 0;
}

/** @} */ // End of DEBUG_PORT

/*******************************************************************************
 * End of file
 *******************************************************************************/

/****************************************************************************************
 * @file        debug_port.c
 * @author      Sarath S
 * @date        2026-01-02
 * @version     1.0
 * @brief       Debug port selection and initialization
 *
 * @details
 * Selects and initializes the appropriate debug port (FreeRTOS or Bare-metal)
 * based on compile-time configuration in config.h. Provides a unified API
 * for the debug framework to remain OS-agnostic.
 *
 * @contact     elektronikaembedded@gmail.com
 * @website     https://elektronikaembedded.wordpress.com
 ****************************************************************************************/

#include "config.h"
#include "debug_port.h"

#if DEBUG_USE_FREERTOS
#include "debug_port_freertos.h"
#endif

#if DEBUG_USE_BAREMETAL
#include "debug_port_baremetal.h"
#endif

/****************************** Function definitions ************************************/

/*!----------------------------------------------------------------------------
 * \brief           Initialize debug port
 * \param[in]       port   Pointer to debug_port_t
 * \param[out]      None
 * \param[in/out]   None
 * \return          0 on success, -1 on error
 * \note            Chooses the port implementation based on config.h
 *---------------------------------------------------------------------------*/
int debug_port_init(debug_port_t *port)
{
    if (NULL == port)
    {
        return -1;
    }

#if DEBUG_USE_FREERTOS
    port->debug_port_ops = *debug_port_freertos_ops();
#elif DEBUG_USE_BAREMETAL
    port->debug_port_ops = *debug_port_baremetal_ops();
#else
#error "No debug port selected! Define DEBUG_USE_FREERTOS or DEBUG_USE_BAREMETAL in config.h"
#endif

    if (NULL != port->debug_port_ops.init)
    {
        return port->debug_port_ops.init();
    }

    return 0;
} /* End of this function */

/*!----------------------------------------------------------------------------
 * \brief           Deinitialize debug port
 * \param[in]       port   Pointer to debug_port_t
 * \param[out]      None
 * \param[in/out]   None
 * \return          0 on success, -1 on error
 * \note            Calls the selected port's deinit function if available
 *---------------------------------------------------------------------------*/
int debug_port_deinit(debug_port_t *port)
{
    if (NULL == port)
    {
        return -1;
    }

    if (NULL != port->debug_port_ops.deinit)
    {
        return port->debug_port_ops.deinit();
    }

    return 0;
} /* End of this function */

/****************************** End of this file ****************************************/

/****************************************************************************************
 * @file        debug_transport.c
 * @author      Sarath S
 * @date        2026-01-02
 * @version     1.0
 * @brief       Debug transport implementation
 *
 * @details
 * This file selects and initializes the appropriate debug transport
 * (USB CDC, UART, or future transports) based on compile-time options
 * defined in config.h.
 *
 * The debug core interacts with the selected transport through a
 * standard operations table, keeping it independent of the
 * underlying hardware or communication interface.
 *
 * @contact     elektronikaembedded@gmail.com
 * @website     https://elektronikaembedded.wordpress.com
 ****************************************************************************************/

#include "config.h"
#include "debug_transport.h"

#if DEBUG_USE_USB_CDC
#include "debug_transport_usb_cdc_st.h"
#endif

#if DEBUG_USE_UART
    #if DEBUG_VENDOR_STM32
        #include "debug_transport_uart_st.h"
    #elif DEBUG_VENDOR_NXP
        #include "debug_transport_uart_nxp.h"
    #elif DEBUG_VENDOR_TI
        #include "debug_transport_uart_ti.h"
    #else
        #error "No UART transport vendor selected! Define DEBUG_VENDOR_STM32/NXP/TI in config.h"
    #endif
#endif

/****************************** Function definitions ************************************/

/*!----------------------------------------------------------------------------
 * \brief           Initialize the debug transport layer
 * \param[in]       transport   Pointer to a debug_transport_hal_t instance
 * \param[out]      None
 * \param[in/out]   None
 * \return          0 on successful initialization
 *                  -1 if transport pointer is NULL
 * \note
 * Chooses the transport backend (USB CDC or UART) based on config.h settings.
 * If the selected transport provides an init function, it will be called.
 * This allows the debug core to remain independent of the underlying interface.
 *---------------------------------------------------------------------------*/
int debug_transport_init(debug_transport_hal_t *transport)
{
    if (NULL == transport)
    {
        return -1;
    }

#if DEBUG_USE_USB_CDC
    transport->debug_transport_ops = *debug_transport_usb_cdc_ops();
#elif DEBUG_USE_UART
    #if DEBUG_VENDOR_STM32
        transport->debug_transport_ops = *debug_transport_uart_st_ops();
    #elif DEBUG_VENDOR_NXP
        transport->debug_transport_ops = *debug_transport_uart_nxp_ops();
    #elif DEBUG_VENDOR_TI
        transport->debug_transport_ops = *debug_transport_uart_ti_ops();
    #else
        #error "No UART transport vendor selected!"
    #endif
#else
    #error "No debug transport selected! Define DEBUG_USE_USB_CDC or DEBUG_USE_UART in config.h"
#endif

    if (NULL != transport->debug_transport_ops.init)
    {
        return transport->debug_transport_ops.init();
    }

    return 0;
} /* End of debug_transport_init() */

/*!----------------------------------------------------------------------------
 * \brief           Deinitialize the debug transport layer
 * \param[in]       transport   Pointer to a debug_transport_hal_t instance
 * \param[out]      None
 * \param[in/out]   None
 * \return          0 on successful deinitialization
 *                  -1 if transport pointer is NULL
 * \note
 * If the selected transport provides a deinit function, it will be called.
 * Otherwise, this function returns 0, allowing the debug framework to
 * cleanly release resources without knowing the transport implementation.
 *---------------------------------------------------------------------------*/
int debug_transport_deinit(debug_transport_hal_t *transport)
{
    if (NULL == transport)
    {
        return -1;
    }

    if (NULL != transport->debug_transport_ops.deinit)
    {
        return transport->debug_transport_ops.deinit();
    }

    return 0;
} /* End of debug_transport_deinit() */

/****************************** End of file *********************************************/

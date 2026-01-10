/**
 * @file      debug_transport.c
 * @brief     Debug transport selection and initialization
 * @version   1.0.0
 * @date      2026-01-02
 * @author    Sarath S
 *
 * @details
 * This module selects and initializes the appropriate debug transport
 * based on compile-time configuration options defined in config.h.
 *
 * Supported transports:
 *   - USB CDC
 *   - UART (STM32, NXP, TI)
 *
 * The debug core interacts with the selected transport exclusively
 * through a transport HAL operations table, ensuring portability and
 * independence from vendor-specific implementations.
 *
 * The transport layer may optionally provide init and deinit routines.
 */

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

/*******************************************************************************
 * Public Function Definitions
 *******************************************************************************/

/**
 * @brief Initialize the debug transport layer.
 *
 * @param[in,out] transport Pointer to a debug transport HAL instance.
 *
 * @retval 0   Transport initialized successfully.
 * @retval -1  Invalid transport pointer or initialization failure.
 *
 * @note
 * The transport backend (USB CDC or UART) is selected based on
 * compile-time configuration macros defined in config.h.
 *
 * If the selected transport provides an init() operation, it will
 * be invoked here.
 */
int debug_transport_init(debug_transport_hal_t *transport)
{
    if(NULL == transport)
    {
        return -1;
    }

#if DEBUG_USE_USB_CDC
    transport->ops = debug_transport_usb_cdc_ops();
#elif DEBUG_USE_UART
    #if DEBUG_VENDOR_STM32
    	transport->ops =  debug_transport_uart_st_ops();
    #elif DEBUG_VENDOR_NXP
    	transport->ops =  = debug_transport_uart_nxp_ops();
    #elif DEBUG_VENDOR_TI
    	transport->ops =  = debug_transport_uart_ti_ops();
    #else
        #error "No UART transport vendor selected!"
    #endif
#else
    #error "No debug transport selected! Define DEBUG_USE_USB_CDC or DEBUG_USE_UART in config.h"
#endif

    if(NULL != transport->ops->init)
    {
        return transport->ops->init();
    }

    return 0;
}/* End of debug_transport_init() */

/**
 * @brief Deinitialize the debug transport layer.
 *
 * @param[in,out] transport Pointer to a debug transport HAL instance.
 *
 * @retval 0   Transport deinitialized successfully.
 * @retval -1  Invalid transport pointer or deinitialization failure.
 *
 * @note
 * If the selected transport does not provide a deinit() operation,
 * this function returns success without performing any action.
 */
int debug_transport_deinit(debug_transport_hal_t *transport)
{
    if (NULL == transport)
    {
        return -1;
    }

    if(NULL != transport->ops->deinit)
    {
        return transport->ops->deinit();
    }

    return 0;
}/* End of debug_transport_deinit() */

/*******************************************************************************
 * End of file
 *******************************************************************************/

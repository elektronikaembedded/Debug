/**
 * @file      debug_transport_usb_cdc_st.h
 * @brief     USB CDC debug transport interface (STM32)
 * @version   1.0.0
 * @date      2025-08-07
 * @author    Sarath S
 *
 * @details
 * This header declares the USB CDC debug transport layer used by the
 * debug framework on STM32 platforms.
 *
 * It provides access to a transport operations table that implements
 * the standard debug transport interface, allowing debug output to be
 * sent over USB CDC.
 *
 * The debug core retrieves this operations table during initialization
 * and remains independent of the underlying USB CDC implementation.
 *
 * @par Contact
 * elektronikaembedded@gmail.com
 *
 * @par Website
 * https://elektronikaembedded.wordpress.com
 */

#ifndef DEBUG_TRANSPORT_USB_CDC_ST_H
#define DEBUG_TRANSPORT_USB_CDC_ST_H

#include "config.h"

#if DEBUG_USE_USB_CDC

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Includes
 *******************************************************************************/
#include "common.h"
#include "debug_transport.h"   /**< Required for debug_transport_ops_t */

/*******************************************************************************
 * Public Functions
 *******************************************************************************/

/**
 * @brief Get USB CDC debug transport operations.
 *
 * @return Pointer to the USB CDC transport operations table.
 *
 * @note
 * This function is used by the debug transport selector during
 * initialization to obtain the function pointers for the USB CDC
 * transport backend.
 */
const debug_transport_ops_t *debug_transport_usb_cdc_ops(void);

#ifdef __cplusplus
}
#endif

#endif /* DEBUG_USE_USB_CDC */
#endif /* DEBUG_TRANSPORT_USB_CDC_ST_H */

/*******************************************************************************
 * End of file
 *******************************************************************************/

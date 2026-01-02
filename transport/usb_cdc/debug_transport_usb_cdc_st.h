/****************************************************************************************
 * @file        debug_transport_usb_cdc_st.h
 * @author      Sarath S
 * @date        2025-08-07
 * @version     1.0
 * @brief       USB CDC debug transport interface
 *
 * @details
 * Declares the USB CDC debug transport layer used by the debug framework.
 * Provides functions to initialize and send debug data over USB.
 *
 * @contact     elektronikaembedded@gmail.com
 * @website     https://elektronikaembedded.wordpress.com
 ****************************************************************************************/

#ifndef DEBUG_TRANSPORT_USB_CDC_ST_H
#define DEBUG_TRANSPORT_USB_CDC_ST_H

#include "config.h"

#if DEBUG_USE_USB_CDC

#ifdef __cplusplus
extern "C" {
#endif

/****************************** Header include files ************************************/
#include "common.h"
#include "debug_transport.h"   /*!< Required for debug_transport_ops_t */

/****************************** Function prototypes ************************************/

/*!----------------------------------------------------------------------------
 * \brief           Get USB CDC debug transport operations
 * \param[in]       None
 * \param[out]      None
 * \param[in/out]   None
 * \return          Pointer to USB CDC transport operations table
 *
 * \note
 * This is used by the debug core during initialization to get
 * the function pointers for USB CDC transport.
 *---------------------------------------------------------------------------*/
const debug_transport_ops_t *debug_transport_usb_cdc_ops(void);

#ifdef __cplusplus
}
#endif

#endif /* DEBUG_USE_USB_CDC */
#endif /* DEBUG_TRANSPORT_USB_CDC_H */

/****************************** End of file *********************************************/

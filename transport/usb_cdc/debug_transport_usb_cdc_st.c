/****************************************************************************************
 * @file        debug_transport_usb_cdc_st.c
 * @author      Sarath S
 * @date        2025-08-15
 * @version     1.0
 * @brief       USB CDC debug transport implementation
 *
 * @details
 * Implements the USB CDC debug transport layer.
 * Provides init, deinit, and write operations required by the
 * debug framework to send log data over USB CDC.
 *
 * @contact     elektronikaembedded@gmail.com
 * @website     https://elektronikaembedded.wordpress.com
 ****************************************************************************************/

#include "config.h"

#if DEBUG_USE_USB_CDC

/****************************** Header include files ************************************/
#include "debug_transport_usb_cdc_st.h"
#include "debug_transport.h"
#include "usbd_cdc_if.h"
#include "usbd_def.h"

/****************************** Static function prototypes ******************************/
static int usb_cdc_init(void);
static int usb_cdc_deinit(void);
static int usb_cdc_write(const uint8_t *data, size_t len);

/****************************** Static variable definitions ******************************/
static const debug_transport_ops_t DEBUG_TRANSPORT_USB_CDC =
{
    .init   = usb_cdc_init,
    .deinit = usb_cdc_deinit,
    .write  = usb_cdc_write,
};

/****************************** Function definitions ************************************/

/*!----------------------------------------------------------------------------
 * \brief           Initialize USB CDC debug transport
 * \param[in]       None
 * \param[out]      None
 * \param[in/out]   None
 * \return          0 on success
 * \note            USB device stack is expected to be initialized externally
 *---------------------------------------------------------------------------*/
static int usb_cdc_init(void)
{
    /* USB device already initialized in main() */
    return 0;
} /* End of this function */

/*!----------------------------------------------------------------------------
 * \brief           Deinitialize USB CDC debug transport
 * \param[in]       None
 * \param[out]      None
 * \param[in/out]   None
 * \return          0 on success
 * \note            USB device stack is expected to be initialized externally
 *---------------------------------------------------------------------------*/
static int usb_cdc_deinit(void)
{
    /* USB device already initialized in main() */
    return 0;
} /* End of this function */

/*!----------------------------------------------------------------------------
 * \brief           Write debug data over USB CDC
 * \param[in]       data   Pointer to data buffer
 * \param[in]       len    Number of bytes to transmit
 * \param[out]      None
 * \param[in/out]   None
 * \return          Number of bytes written, or -1 on error
 * \note            Returns -1 if USB CDC is busy or parameters are invalid
 *---------------------------------------------------------------------------*/
static int usb_cdc_write(const uint8_t *data, size_t len)
{
    if ((NULL == data) || (0U == len))
    {
        return -1;
    }

    /* CDC_Transmit_FS returns USBD_BUSY if previous transfer is ongoing */
    if (USBD_OK == CDC_Transmit_FS((uint8_t *)data, len))
    {
        return (int)len;
    }

    return -1;
} /* End of this function */

/*!----------------------------------------------------------------------------
 * \brief           Get USB CDC debug transport operations
 * \param[in]       None
 * \param[out]      None
 * \param[in/out]   None
 * \return          Pointer to USB CDC transport operations table
 * \note            Used by debug core during initialization
 *---------------------------------------------------------------------------*/
const debug_transport_ops_t *debug_transport_usb_cdc_ops(void)
{
    return &DEBUG_TRANSPORT_USB_CDC;
} /* End of this function */

#endif /* DEBUG_USE_USB_CDC */

/****************************** End of file *********************************************/

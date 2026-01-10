/**
 * @file      debug_transport_usb_cdc_st.c
 * @brief     USB CDC debug transport implementation (STM32)
 * @version   1.0.0
 * @date      2025-08-15
 * @author    Sarath S
 *
 * @details
 * This module implements the USB CDC debug transport layer for STM32
 * platforms.
 *
 * It provides the init, deinit, and write operations required by the
 * debug framework to transmit log data over USB CDC.
 *
 * The USB device stack is expected to be initialized externally by
 * the application.
 *
 * @par Contact
 * elektronikaembedded@gmail.com
 *
 * @par Website
 * https://elektronikaembedded.wordpress.com
 */

#include "config.h"

#if DEBUG_USE_USB_CDC

/*******************************************************************************
 * Includes
 *******************************************************************************/
#include "debug_transport_usb_cdc_st.h"
#include "debug_transport.h"
#include "usbd_cdc_if.h"
#include "usbd_def.h"

/*******************************************************************************
 * Private Function Prototypes (Static)
 *******************************************************************************/
static int usb_cdc_init(void);
static int usb_cdc_deinit(void);
static int usb_cdc_write(const uint8_t *data, size_t len);

/*******************************************************************************
 * Private Variables (Static)
 *******************************************************************************/

/**
 * @brief USB CDC transport operations table
 */
static const debug_transport_ops_t DEBUG_TRANSPORT_USB_CDC =
{
    .init   = usb_cdc_init,
    .deinit = usb_cdc_deinit,
    .write  = usb_cdc_write,
};

/*******************************************************************************
 * Private Function Definitions (Static)
 *******************************************************************************/

/**
 * @brief Initialize the USB CDC debug transport.
 *
 * @retval 0  Initialization successful.
 *
 * @note
 * The USB device stack is expected to be initialized externally
 * (typically in main()).
 */
static int usb_cdc_init(void)
{
    /* USB device already initialized in main() */
    return 0;
}/* End of usb_cdc_init() */

/**
 * @brief Deinitialize the USB CDC debug transport.
 *
 * @retval 0  Deinitialization successful.
 *
 * @note
 * The USB device stack is expected to be managed externally.
 */
static int usb_cdc_deinit(void)
{
    /* USB device already initialized in main() */
    return 0;
}/* End of usb_cdc_deinit() */

/**
 * @brief Write debug data over USB CDC.
 *
 * @param[in] data Pointer to data buffer.
 * @param[in] len  Number of bytes to transmit.
 *
 * @retval >=0  Number of bytes written.
 * @retval -1   Transmission failed or USB busy.
 *
 * @note
 * Returns -1 if the USB CDC interface is busy or if the
 * input parameters are invalid.
 */
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
}/* End of usb_cdc_write() */

/*******************************************************************************
 * Public Function Definitions
 *******************************************************************************/

/**
 * @brief Get USB CDC debug transport operations.
 *
 * @return Pointer to the USB CDC transport operations table.
 *
 * @note
 * Used by the debug transport selector during framework initialization.
 */
const debug_transport_ops_t *debug_transport_usb_cdc_ops(void)
{
    return &DEBUG_TRANSPORT_USB_CDC;
}/* End of debug_transport_usb_cdc_ops() */

#endif /* DEBUG_USE_USB_CDC */

/*******************************************************************************
 * End of file
 *******************************************************************************/
